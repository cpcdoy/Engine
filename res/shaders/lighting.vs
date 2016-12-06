#version 330 core
in vec3 position;
in vec2 tex_coords_;

out vec2 tex_coords;

void main()
{
  gl_Position = vec4(position, 1.0f);
  tex_coords = tex_coords_;
}
