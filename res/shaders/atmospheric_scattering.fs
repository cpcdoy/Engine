#version 140

uniform vec3 v3LightPos;

const float g  = -0.98f;
const float g2 =  0.9604f;

in vec3 v3Direction;
in vec3 frontSecColor;
in vec3 frontColor;

void main(void)
{
    float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
    float fRayleighPhase = 0.75 * (1.0 + fCos*fCos);
    float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);

    gl_FragColor.rgb = fRayleighPhase * vec3(0.1, 0.2, 0.5) + fMiePhase * vec3(0.5, 0.2, 0.1);
    gl_FragColor.a = gl_FragColor.b;
}
