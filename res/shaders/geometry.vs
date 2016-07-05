#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coords_;
layout (location = 2) in vec3 normal;

out vec3 frag_pos;
out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  mat4 model1;
	model1 = model;

  vec4 view_pos = view * model * vec4(pos, 1.0f);
  frag_pos = view_pos.xyz;
  gl_Position = projection * view_pos;
  tex_coords = tex_coords_;
}
