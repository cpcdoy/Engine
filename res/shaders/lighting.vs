#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coords_;
layout (location = 2) in vec3 normal_;

out vec2 tex_coords;

out VS_OUT
{
    vec3 frag_pos;
    vec3 normal;
    vec2 tex_coords;
    vec4 frag_posLightSpace;
} vs_out;

uniform bool instanciate;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 light_space_matrix;

//uniform float u_time;

void main()
{	
  gl_Position = projection * view * model * vec4(position, 1.0f);
  vs_out.frag_pos = vec3(model * vec4(position, 1.0));
  vs_out.normal = transpose(inverse(mat3(model))) * normal_;
  vs_out.tex_coords = tex_coords_;
  vs_out.frag_posLightSpace = light_space_matrix * vec4(vs_out.frag_pos, 1.0);
}
