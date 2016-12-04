#version 140

in vec3 position_;

uniform mat4 VP;
uniform mat4 model;

void main()
{
  gl_Position = VP * model * vec4(position_, 1.0f);
}
