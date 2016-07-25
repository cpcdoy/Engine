#version 140
in vec3 position;
in vec2 tex_coords_;
in vec3 normal_;

out vec3 frag_pos;
out vec2 tex_coords;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  vec4 viewPos = view * model * vec4(position, 1.0f);
  frag_pos = viewPos.xyz;
  gl_Position = projection * viewPos;
  tex_coords = tex_coords_;

  mat3 normal_matrix = transpose(inverse(mat3(view * model)));
  normal = normal_matrix * normal_;
}
