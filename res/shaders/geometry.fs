#version 140
out vec4 g_position_depth;
out vec3 g_normal;
out vec3 g_albedo;
out vec3 g_metalness_roughness_baked_ao;

in vec2 tex_coords;
in vec3 frag_pos;
in vec3 normal;
in vec3 view_dir;

uniform sampler2D albedo_map;
uniform sampler2D metalness_map;
uniform sampler2D roughness_map;
uniform sampler2D baked_ao_map;
uniform sampler2D normal_map;

const float NEAR = 0.01f;
const float FAR = 1000.0f;
float linearize_depth(float depth)
{
  float z = depth * 2.0 - 1.0; // Back to NDC
  return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

mat3 cotangent_frame(vec3 n, vec3 p, vec2 uv)
{
  vec3 dp1 = dFdx(p);
  vec3 dp2 = dFdy(p);
  vec2 duv1 = dFdx(uv);
  vec2 duv2 = dFdy(uv);

  vec3 dp2perp = cross(dp2, n);
  vec3 dp1perp = cross(n, dp1);

  vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
  vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

  float inv_max = inversesqrt(max(dot(T, T), dot(B, B)));
  return mat3(T * inv_max, B * inv_max, n);
}

vec3 perturb_normal(vec3 n, vec3 v, vec2 uv)
{
  vec3 n_map = texture2D(normal_map, tex_coords).rgb;

  n_map = n_map * 255f / 127f - 128f / 127f;

  mat3 TBN = cotangent_frame(n, -v, tex_coords);

  return normalize(TBN * n_map);
}

void main()
{
  g_position_depth.xyz = frag_pos;

  g_normal = normal;
  g_normal = perturb_normal(g_normal, view_dir, tex_coords);

  g_albedo = texture2D(albedo_map, tex_coords).rgb;

  float metalness = texture2D(metalness_map, tex_coords).r;
  float roughness = texture2D(roughness_map, tex_coords).r;
  float baked_ao = texture2D(baked_ao_map, tex_coords).r;
  g_metalness_roughness_baked_ao = vec3(metalness, roughness, baked_ao);
}
