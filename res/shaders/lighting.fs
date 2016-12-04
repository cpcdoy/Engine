#version 330
//#define BRDF_FD_OREN_NAYAR
#define BRDF_FD_LAMBERT

vec3 sh_color;

const float C1 = 0.429043;
const float C2 = 0.511664;
const float C3 = 0.743125;
const float C4 = 0.886227;
const float C5 = 0.247708;

// Funston Beach Sunset
const vec3 L00  = vec3(0.68, 0.69, 0.70);
const vec3 L1m1 = vec3(0.32, 0.37, 0.44);
const vec3 L10  = vec3(0.17, 0.17, 0.17);
const vec3 L11  = vec3(0.45, 0.42, 0.34);
const vec3 L2m2 = vec3(0.17, 0.17, 0.15);
const vec3 L2m1 = vec3(0.08, 0.09, 0.10);
const vec3 L20  = vec3(0.03, 0.03, 0.01);
const vec3 L21  = vec3(0.16, 0.14, 0.10);
const vec3 L22  = vec3(0.37, 0.31, 0.20);

out vec4 frag_color;

in vec2 tex_coords;

uniform mat4 light_space_matrix;

uniform sampler2D shadow_map;
uniform sampler2D ao_map;
uniform sampler2D diffuse_map;
uniform sampler2D metalness_roughness_baked_ao_map;
uniform sampler2D g_position_depth;
uniform sampler2D g_normal;

uniform vec2 screen_res;

uniform vec3 light_pos;
uniform vec3 view_pos;

uniform mat4 view;
uniform mat4 projection;

#define PI 3.1415926535897932
#define ONE_OVER_PI 0.318309

vec2 ss_coords = tex_coords;

vec3 metalness_roughness_baked_ao = texture2D(metalness_roughness_baked_ao_map, ss_coords).rgb;
float metalness = clamp(metalness_roughness_baked_ao.r, 0.02, 0.99);//1.0;
float roughness = 1.0 - max(metalness_roughness_baked_ao.g, 0.001);//0.68;

vec4 frag_info_fs = texture(g_position_depth, ss_coords).rgba;
vec3 frag_pos_fs = (inverse(view) * vec4(frag_info_fs.rgb, 1.0)).rgb;
float depth_fs = frag_info_fs.z;
vec3 normal_fs = texture(g_normal, ss_coords).rgb;
vec4 frag_pos_fs_light_space = light_space_matrix * vec4(frag_pos_fs, 1.0);

vec4 base_color = vec4(1.0);

float ggx_chi(float h_dot_n)
{
  return h_dot_n > 0.0 ? 1.0 : 0.0;
}

// GGX
// D
// alpha : roughness
// n : normal
// h : half vector
float ggx_distribution(float alpha, float n_dot_h)
{
  alpha *= alpha; //Disney's reparametrization of alpha
  float alpha_2 = alpha * alpha;
  float n_dot_h_2 = n_dot_h * n_dot_h;
  float d = n_dot_h_2 * (alpha_2 - 1.0) + 1.0;

  return alpha_2 / max(PI * d * d, 1e-8);
}

// Gp
// w : view
// h : half
// n : normal
// alpha : roughness
/*float ggx_partial_geometry_term(vec3 w, vec3 h, vec3 n, float alpha)
{
  float w_dot_h = clamp(dot(w, h), 0.0, 1.0);
  float w_dot_n = clamp(dot(w, n), 0.0, 1.0);
  float chi_dots = ggx_chi(w_dot_h / w_dot_n);
  float alpha_2 = alpha * alpha;
  float w_dot_h_2 = w_dot_h * w_dot_h;
  float sqrt_res = sqrt(1 + (alpha_2 * ((1 - w_dot_h_2) / w_dot_h_2)));

  return (chi_dots * 2) / (1 + sqrt_res);
}*/

vec3 fresnel_schlick(float cos_t, vec3 f0)
{
  //UE4's Spherical Gaussian approximation
  return f0 + (1 - f0) * pow(2, (-5.55473 * cos_t - 6.98316) * cos_t);
  //return f0 + (1 - f0) * pow((1.0 - cos_t), 5.0);
}

float schlick_geometry(float n_dot_l, float n_dot_v, float roughness)
{
  float a = roughness + 1.0; //Disney: reduced "hotness"
  float k = a * a * 0.125;
  float one_minus_k = 1 - k;

  float vis_schlick_v = n_dot_v * one_minus_k + k;
  float vis_schlick_l = n_dot_l * one_minus_k + k;

  //UE4
  return (n_dot_v / vis_schlick_v) * (n_dot_l / vis_schlick_l);
}

//BRDFs
#ifdef BRDF_FD_LAMBERT
vec4 brdf_lambert()
{
  vec4 color = base_color;

  color.rgb = mix(color.rgb, vec3(0.0), metalness);
  color.rgb *= ONE_OVER_PI;

  return color;
}

#elif defined(BRDF_FD_OREN_NAYAR)
vec4 brdf_oren_nayar(float n_dot_v, float n_dot_l, vec3 light_dir, vec3 view_dir, vec3 n)
{
  float angle_v_n = acos(n_dot_v);
  float angle_l_n = acos(n_dot_l);

  float alpha = max(angle_v_n, angle_l_n);
  float beta = min(angle_v_n, angle_l_n);
  float gamma = dot(view_dir - n * n_dot_v, light_dir - n * n_dot_l);

  float roughness_2 = roughness * roughness;

  float A = 1.0 - 0.5 * (roughness_2 / (roughness_2 + 0.57));
  float B = 0.45 * (roughness_2 / (roughness_2 + 0.09));
  float C = sin(alpha) * tan(beta);

  float L1 = max(0.0, n_dot_l) * (A + B * max(0.0, gamma) * C);

  vec4 color = base_color;
  color.rgb = mix(color.rgb, vec3(0.0), metalness);

  return vec4(color.rgb * vec3(L1), 1.0);
}
#endif

vec3 brdf_cook_torrance(float v_dot_h, float n_dot_h, float n_dot_v, float n_dot_l, float alpha)
{
  vec3 f0 = mix(vec3(0.04), base_color.rgb, metalness);
  vec3 f = fresnel_schlick(v_dot_h, f0);

  float g = schlick_geometry(n_dot_l, n_dot_v, alpha);
  float d = ggx_distribution(alpha, n_dot_h);

  return f * g * d;
}

//Shadow mapping
float compute_shadows(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  if(projCoords.z > 1.0)
    return 0.0;

  float closestDepth = texture(shadow_map, projCoords.xy).r;
  float currentDepth = projCoords.z;

  float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.005);

  //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
  // PCF
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
  for(int x = -1; x <= 1; ++x)
    for(int y = -1; y <= 1; ++y)
    {
      float pcfDepth = texture(shadow_map, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
    }
  shadow /= 9.0;

  return shadow;
}

// Uncharted 2's filmic tonemapping operators
vec3 uncharted_tonemap(vec3 x)
{
   const float A = 0.15;
   const float B = 0.50;
   const float C = 0.10;
   const float D = 0.20;
   const float E = 0.02;
   const float F = 0.30;
   const float W = 11.2;

   return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 exposure(vec3 color)
{
  const float exposure_bias = 2.0f;
  vec3 curr = uncharted_tonemap(exposure_bias * color);

  const vec3 W = vec3(11.2);
  vec3 white_scale = 1.0 / uncharted_tonemap(W);

  return curr * white_scale;
}

void main()
{
  /*vec2 floor = floor(gl_FragCoord.xy * 0.25f) * 0.5f;
  float res = -fract(floor.x + floor.y);
  if (res == 0.0f)
    discard;*/

  vec3 light_color = vec3(0.98, 0.83, 0.64);

  sh_color = C1 * L22 * (normal_fs.x * normal_fs.x - normal_fs.y * normal_fs.y) +
             C3 * L20 * normal_fs.z * normal_fs.z +
             C4 * L00 - C5 * L20 +
             2.0 * C1 * (L2m2 * normal_fs.x * normal_fs.y + L21 * normal_fs.x * normal_fs.z + L2m1 * normal_fs.y * normal_fs.z) +
             2.0 * C2 * (L11 * normal_fs.x + L1m1 * normal_fs.y + L10 * normal_fs.z);

  sh_color *= 2.5f;

  vec3 baked_ao = vec3(metalness_roughness_baked_ao.b);
  vec3 color = texture(diffuse_map, ss_coords).rgb;
  base_color = vec4(color, 1.0);

  vec3 lightDir = mat3(view) * light_pos - frag_pos_fs;
  vec3 l = normalize(lightDir);

  vec3 viewDirUnNorm = mat3(view) * view_pos - frag_pos_fs;
  vec3 v = normalize(viewDirUnNorm);
  vec3 v_pos = normalize(-frag_pos_fs);

  vec3 n = normal_fs;

  vec3 h = normalize(v + l);

  float n_dot_v = clamp(dot(n, v), 0.0, 1.0);
  float n_dot_l = clamp(dot(n, l), 0.0, 1.0);
  float n_dot_h = clamp(dot(n, h), 0.0, 1.0);
  float v_dot_h = clamp(dot(v, h), 0.0, 1.0);

#ifdef BRDF_FD_LAMBERT
  vec4 fd = brdf_lambert();
#elif defined(BRDF_FD_OREN_NAYAR)
  vec4 fd = brdf_oren_nayar(n_dot_v, n_dot_l, lightDir, v, n);
#endif
  vec3 fs = brdf_cook_torrance(v_dot_h, n_dot_h, n_dot_v, n_dot_l, roughness);

  vec3 ambient = vec3(0.2 * texture(ao_map, ss_coords).r * baked_ao);

  float shadow = compute_shadows(frag_pos_fs_light_space, n, l);
  vec3 lighting = (ambient + ((fd.rgb * fd.a + fs) * n_dot_l * (1.0 - shadow))) * light_color * sh_color * color;

  frag_color = vec4(exposure(lighting), fd.a);
}
