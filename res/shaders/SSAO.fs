#version 330 core
#pragma optionNV(fastmath on)
#pragma optionNV(fastprecision on)
#pragma optionNV(ifcvt none)
#pragma optionNV(inline all)
#pragma optionNV(strict on)
#pragma optionNV(unroll all)

out vec4 frag_color;
in vec2 tex_coords;

uniform sampler2D g_position_depth;
uniform sampler2D g_normal;
uniform sampler2D tex_noise;

float radius = 1.0;

#define KERNEL_SIZE 32
uniform vec3 samples[KERNEL_SIZE];

const vec2 noise_scale = vec2(1280 /4.0f, 720.0f / 4.0f); 

uniform mat4 projection;

#define PI 3.14159

void main()
{
  vec3 fragPos = texture2D(g_position_depth, tex_coords).xyz;
  vec3 normal = texture2D(g_normal, tex_coords).rgb;
  vec3 random_vec = texture2D(tex_noise, tex_coords * noise_scale).xyz;

  vec3 tangent = normalize(random_vec - normal * dot(random_vec, normal));
  vec3 bitangent = cross(normal, tangent);
  mat3 TBN = mat3(tangent, bitangent, normal);

  float occlusion = 0.0;
  for (int i = 0; i < KERNEL_SIZE; ++i)
  {
    vec3 sample = TBN * samples[i];
    sample = fragPos + sample * radius;

    vec4 offset = vec4(sample, 1.0);
    offset = projection * offset;
    offset.xyz /= offset.w;
    offset.xyz = offset.xyz * 0.5 + 0.5;

    /*if (dot(normalize(offset.xyz), normal) < 0.15)
      {
      frag_color = 1.0;
      return;
      }*/

    float sample_depth = texture2D(g_position_depth, offset.xy).z;

    float range_check = smoothstep(0.0, 1.0, radius / abs(fragPos.z -
          sample_depth));
    occlusion += (sample_depth >= sample.z ? 1.0 : 0.0)  * range_check;
  }
  occlusion = 1.0 - (occlusion / KERNEL_SIZE);

  frag_color = vec4(occlusion);
}
