#version 330 core
out vec4 gPositionDepth;

in vec2 tex_coords;
in vec3 frag_pos;

const float NEAR = -1.0f;
const float FAR = 1000.0f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));	
}

void main()
{
  gPositionDepth.xyz = vec3(0.0, 0.0, 0.0);
}

