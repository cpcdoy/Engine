#version 330

layout (location = 0) in vec3 position;

#define PI 3.1415926535897932

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 v3CameraPos;// The camera's current position
uniform vec3 v3LightPos;// The direction vector to the light source
vec3 v3InvWavelength = vec3(1.0f / pow(0.650f, 4),
                            1.0f / pow(0.570f, 4),
                            1.0f / pow(0.475f, 4));
uniform float fCameraHeight;// The camera's current height
uniform float fCameraHeight2;// fCameraHeight^2

float fOuterRadius = 10.25f;// The outer (atmosphere) radius
float fOuterRadius2 = fOuterRadius * fOuterRadius;// fOuterRadius^2
float fInnerRadius = 10.0f;// The inner (planetary) radius
float fInnerRadius2 = fInnerRadius * fInnerRadius;// fInnerRadius^2
float Km = 0.0025f;
float Kr = 0.0010f;
float ESun = 10.0f;
float fKrESun = Kr * ESun;
float fKmESun = Km * ESun;
float fKr4PI = Kr * 4 * PI;
float fKm4PI = Km * 4 * PI;
float fScale = 1 / (fOuterRadius - fInnerRadius);
float fScaleDepth = 0.25f;// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
float fScaleOverScaleDepth =  fScale / fScaleDepth;

const int nSamples = 3;
const float fSamples = 3.0;

out vec3 v3Direction;
out vec3 frontSecColor;
out vec3 frontColor;

float scale(float fCos)
{
    float x = 1.0 - fCos;
    return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main(void)
{
    vec3 v3Pos = position;
    vec3 v3Ray = v3Pos - v3CameraPos;
    float fFar = length(v3Ray);
    v3Ray /= fFar;

    // Calculate the ray's starting position, then calculate its scattering offset
    vec3 v3Start = v3CameraPos;
    float fHeight = length(v3Start);
    float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
    float fStartAngle = dot(v3Ray, v3Start) / fHeight;
    float fStartOffset = fDepth*scale(fStartAngle);

    // Initialize the scattering loop variables
    //gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
    float fSampleLength = fFar / fSamples;
    float fScaledLength = fSampleLength * fScale;
    vec3 v3SampleRay = v3Ray * fSampleLength;
    vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

    // Now loop through the sample rays
    vec3 v3FrontColor = vec3(0.0, 0.0, 0.0);
    for(int i=0; i<nSamples; i++)
    {
        float fHeight = length(v3SamplePoint);
        float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
        float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
        float fCameraAngle = 1.0f;
        float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
        vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
        v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
    }

    // Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
    frontSecColor = v3FrontColor * fKmESun;
    frontColor = v3FrontColor * (v3InvWavelength * fKrESun);
    gl_Position = projection * view * model * vec4(position, 1.0f);
    v3Direction = v3CameraPos - v3Pos;
}
