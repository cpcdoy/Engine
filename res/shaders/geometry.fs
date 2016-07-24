#version 330 core
layout (location = 0) out vec4 g_position_depth;
layout (location = 1) out vec3 g_normal;

in vec2 tex_coords;
in vec3 frag_pos;
in vec3 normal;

const float NEAR = -1.0f;
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
}
