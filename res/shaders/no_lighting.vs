#version 140

in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  vec4 view_pos = view * model * vec4(pos, 1.0f);
  gl_Position = projection * view_pos;
}
