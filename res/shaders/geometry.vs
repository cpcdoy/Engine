#version 140
in vec3 position_;
in vec2 tex_coords_;
in vec3 normal_;

out vec3 frag_pos;
out vec2 tex_coords;
out vec3 normal;

uniform mat4 model;

void main()
{
  frag_pos = (model * vec4(position_, 1.0f)).xyz;
  tex_coords = tex_coords_;

  normal = (model * vec4(normal_, 1.0)).xyz;
}
