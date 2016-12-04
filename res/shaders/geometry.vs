#version 140
in vec3 position_;
in vec2 tex_coords_;
in vec3 normal_;

out vec3 frag_pos;
out vec2 tex_coords;
out vec3 normal;
out vec3 view_dir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model_view;
uniform mat3 normal_matrix;

void main()
{
  vec4 view_pos = model_view * vec4(position_, 1.0f);
  frag_pos = view_pos.xyz;
  gl_Position = projection * view_pos;
  tex_coords = tex_coords_;

  view_dir = frag_pos - position_;

  normal = normal_matrix * normal_;
}
