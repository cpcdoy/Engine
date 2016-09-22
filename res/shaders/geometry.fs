#version 140
out vec4 g_position_depth;
out vec3 g_normal;
out vec3 g_albedo;
out vec3 g_metalness_roughness_baked_ao;

in vec2 tex_coords;
in vec3 frag_pos;
in vec3 normal;

uniform sampler2D albedo_map;
uniform sampler2D metalness_map;
uniform sampler2D roughness_map;
uniform sampler2D baked_ao_map;

const float NEAR = 0.01f;
const float FAR = 1000.0f;
float linearize_depth(float depth)
{
  float z = depth * 2.0 - 1.0; // Back to NDC
  return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

void main()
{
  g_position_depth.xyz = frag_pos;
  g_position_depth.a = linearize_depth(gl_FragCoord.z);

  g_normal = normalize(normal);
  g_albedo = texture2D(albedo_map, tex_coords).rgb;

  float metalness = texture2D(metalness_map, tex_coords).r;
  float roughness = texture2D(roughness_map, tex_coords).r;
  float baked_ao = texture2D(baked_ao_map, tex_coords).r;
  g_metalness_roughness_baked_ao = vec3(metalness, roughness, baked_ao);
}
