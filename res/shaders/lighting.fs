#version 430 core

out vec4 frag_color;

in VS_OUT {
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  vec4 frag_pos_light_space;
} fs_in;

layout(binding = 0) uniform sampler2D shadow_map;
layout(binding = 1) uniform sampler2D ao_map;
layout(binding = 2) uniform sampler2D diffuse_map;
layout(binding = 3) uniform sampler2D metalness_map;
layout(binding = 4) uniform sampler2D roughness_map;
layout(binding = 5) uniform sampler2D baked_ao_map;

uniform vec2 screen_res;

uniform vec3 light_pos;
uniform vec3 view_pos;

bool AO = true;

#define PI 3.1415926535897932
#define ONE_OVER_PI 0.318309

float metalness = clamp(texture(metalness_map, fs_in.tex_coords).r, 0.02, 0.99);//1.0;
float roughness = 1.0 - max(texture(roughness_map, fs_in.tex_coords).r, 0.001);//0.68;
float has_texture = 1.0;

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
  float chi_n_dot_h = ggx_chi(n_dot_h);
  float d = (n_dot_h_2 * (alpha_2 + ((1 - n_dot_h_2) / n_dot_h)));
  float d_2 = d * d;

  return (alpha_2 * chi_n_dot_h) / max(PI * d_2, 1e-8);
}

// Gp
// w : view
// h : half
// n : normal
// alpha : roughness
float ggx_partial_geometry_term(vec3 w, vec3 h, vec3 n, float alpha)
{
  float w_dot_h = clamp(dot(w, h), 0.0, 1.0);
  float w_dot_n = clamp(dot(w, n), 0.0, 1.0);
  float chi_dots = ggx_chi(w_dot_h / w_dot_n);
  float alpha_2 = alpha * alpha;
  float w_dot_h_2 = w_dot_h * w_dot_h;
  float sqrt_res = sqrt(1 + (alpha_2 * ((1 - w_dot_h_2) / w_dot_h_2)));

  return (chi_dots * 2) / (1 + sqrt_res);
}

vec3 fresnel_schlick(float cos_t, vec3 f0)
{
  return f0 + (1 - f0) * pow((1.0 - cos_t), 5.0);
}

float schlick_geometry(float n_dot_l, float n_dot_v, float roughness)
{
	// = G_Schlick / (4 * ndotv * ndotl)
	float a = roughness + 1.0;
	float k = a * a * 0.125;

	float vis_schlick_v = n_dot_v * (1 - k) + k;
	float vis_schlick_l = n_dot_l * (1 - k) + k;

	return 0.25 / (vis_schlick_v * vis_schlick_l);
}

//BRDFs
vec4 brdf_lambert(vec2 uv)
{
  vec4 albedo = texture(diffuse_map, uv);
  vec4 color = mix(base_color, albedo, has_texture);

  color.rgb = mix(color.rgb, vec3(0.0), metalness);
  color.rgb *= ONE_OVER_PI;

  return color;
}

vec3 brdf_cook_torrance(float l_dot_h, float n_dot_h, float n_dot_v, float n_dot_l, float alpha)
{
  vec3 f0 = mix(vec3(0.04), base_color.rgb, metalness);
  vec3 f = fresnel_schlick(l_dot_h, f0);

  float g = schlick_geometry(n_dot_l, n_dot_v, alpha);
  float d = ggx_distribution(alpha, n_dot_h);

  return f * g * d;
}

//Shadow mapping
float ShadowCalculation(vec4 fragPosLightSpace)
{
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture2D(shadow_map, projCoords.xy).r; 
  float currentDepth = projCoords.z;

  vec3 normal = normalize(fs_in.normal);
  vec3 lightDir = normalize(light_pos - fs_in.frag_pos);
  float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.005);

  //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
  // PCF
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
  for(int x = -1; x <= 1; ++x)
  {
    for(int y = -1; y <= 1; ++y)
    {
      float pcfDepth = texture2D(shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
      shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
    }    
  }
  shadow /= 9.0;

  if(projCoords.z > 1.0)
    shadow = 0.0;

  return shadow;
}

vec3 gamma(vec3 v)
{
  return pow(v, vec3(2.2));
}

void main()
{           
  float baked_ao = texture(baked_ao_map, fs_in.tex_coords).r;
  vec3 color = texture(diffuse_map, fs_in.tex_coords).rgb * vec3(baked_ao);
  base_color = vec4(color, 1.0);

  vec3 normal = normalize(fs_in.normal);
  vec3 lightColor = vec3(0.0, 0.2, 0.4);
  
  vec3 lightDir = light_pos - fs_in.frag_pos;
  vec3 l = normalize(lightDir);

  vec3 viewDirUnNorm = view_pos - fs_in.frag_pos;
  vec3 v = normalize(viewDirUnNorm);

  vec3 n = normalize(fs_in.normal);

  vec3 h = normalize(v + l);

	float ndotv = clamp(dot(n, v), 0.0, 1.0);
	float ndotl = clamp(dot(n, l), 0.0, 1.0);
	float ndoth = clamp(dot(n, h), 0.0, 1.0);
  float ldoth = clamp(dot(l, h), 0.0, 1.0);

	vec4 fd = brdf_lambert(fs_in.tex_coords);
	vec3 fs = brdf_cook_torrance(ldoth, ndoth, ndotv, ndotl, roughness);

  vec3 ambient = 0.2 * texture2D(ao_map, gl_FragCoord.xy / screen_res).r * color;

  float shadow = ShadowCalculation(fs_in.frag_pos_light_space);
  vec3 lighting = (ambient + (fd.rgb * fd.a + fs) * ndotl * (1.0 - shadow)) * lightColor; //(ambient + (1.0 - shadow) * (diffuse + specular)) * color;

  frag_color = vec4(lighting, fd.a);
}
