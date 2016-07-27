#version 430 core
#define BRDF_FD_OREN_NAYAR

out vec4 frag_color;

in VS_OUT {
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  vec4 frag_pos_light_space;
} fs_in;

uniform sampler2D shadow_map;
uniform sampler2D ao_map;
uniform sampler2D diffuse_map;
uniform sampler2D metalness_map;
uniform sampler2D roughness_map;
uniform sampler2D baked_ao_map;

uniform vec2 screen_res;

uniform vec3 light_pos;
uniform vec3 view_pos;

#define PI 3.1415926535897932
#define ONE_OVER_PI 0.318309

float metalness = clamp(texture(metalness_map, fs_in.tex_coords).r, 0.02, 0.99);//1.0;
float roughness = 1.0 - max(texture(roughness_map, fs_in.tex_coords).r, 0.001);//0.68;

vec4 base_color = vec4(1.0);

float ggx_chi(float h_dot_n)
{
  return h_dot_n > 0.0 ? 1.0 : 0.0;
}

// Night-time GGX
// D
// alpha : roughness
// n : normal
// h : half vector
float ggx_distribution(float alpha, float n_dot_h)
{
  float alpha_2 = alpha * alpha;
  float n_dot_h_2 = n_dot_h * n_dot_h;
  float d = (n_dot_h_2 * (alpha_2 + ((1 - n_dot_h_2) / n_dot_h)));

  return (alpha_2 * ggx_chi(n_dot_h)) / max(PI * d * d, 1e-8);
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
  return f0 + (1 - f0) * pow((1.0 - cos_t), 5.0);
}

float schlick_geometry(float n_dot_l, float n_dot_v, float roughness)
{
	// = G_Schlick / (4 * n_dot_v * n_dot_l)
	float a = roughness + 1.0;
	float k = a * a * 0.125;
  float one_minus_k = 1 - k;

	float vis_schlick_v = n_dot_v * one_minus_k + k;
	float vis_schlick_l = n_dot_l * one_minus_k + k;

	return 0.25 / (vis_schlick_v * vis_schlick_l);
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
vec4 brd_oren_nayar(float n_dot_v, float n_dot_l, vec3 light_dir, vec3 view_dir, vec3 n)
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

  return vec4(base_color.rgb * vec3(L1), 1.0);
}
#endif

vec3 brdf_cook_torrance(float l_dot_h, float n_dot_h, float n_dot_v, float n_dot_l, float alpha)
{
  vec3 f0 = mix(vec3(0.04), base_color.rgb, metalness);
  vec3 f = fresnel_schlick(l_dot_h, f0);

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

void main()
{           
  float baked_ao = texture(baked_ao_map, fs_in.tex_coords).r;
  vec3 color = texture(diffuse_map, fs_in.tex_coords).rgb * vec3(baked_ao);
  base_color = vec4(color, 1.0);

  vec3 normal = normalize(fs_in.normal);
  vec3 light_color = vec3(1.0);
  
  vec3 lightDir = light_pos - fs_in.frag_pos;
  vec3 l = normalize(lightDir);

  vec3 viewDirUnNorm = view_pos - fs_in.frag_pos;
  vec3 v = normalize(viewDirUnNorm);

  vec3 n = normalize(fs_in.normal);

  vec3 h = normalize(v + l);

	float n_dot_v = clamp(dot(n, v), 0.0, 1.0);
	float n_dot_l = clamp(dot(n, l), 0.0, 1.0);
	float n_dot_h = clamp(dot(n, h), 0.0, 1.0);
  float l_dot_h = clamp(dot(l, h), 0.0, 1.0);

#ifdef BRDF_FD_LAMBERT
	vec4 fd = brdf_lambert();
#elif defined(BRDF_FD_OREN_NAYAR)
  vec4 fd = brd_oren_nayar(n_dot_v, n_dot_l, lightDir, v, n);
#endif
	vec3 fs = brdf_cook_torrance(l_dot_h, n_dot_h, n_dot_v, n_dot_l, roughness);

  vec3 ambient = 0.2 * texture(ao_map, gl_FragCoord.xy / screen_res).r * color;

  float shadow = compute_shadows(fs_in.frag_pos_light_space, normal, l);
  vec3 lighting = (ambient + (fd.rgb * fd.a + fs) * n_dot_l * (1.0 - shadow)) * light_color; //(ambient + (1.0 - shadow) * (diffuse + specular)) * color;

  frag_color = vec4(lighting, fd.a);
}
