#version 330 core
out vec4 gPositionDepth;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

const float NEAR = -1.0f;
const float FAR = 1000.0f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));	
}

void main()
{
    /*gPositionDepth.xyz = FragPos;
    gPositionDepth.a = LinearizeDepth(gl_FragCoord.z);
    gNormal = normalize(Normal);
    gAlbedo = vec3(0, 1, 0);*/

    gPositionDepth.xyz = vec3(0.0, 0.0, 0.0);
}

