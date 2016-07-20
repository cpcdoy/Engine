#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coords_;
layout (location = 2) in vec3 normal_;
layout (location = 3) in mat4 a_model;

out vec2 tex_coords;

out VS_OUT
{
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  vec4 frag_pos_light_space;
} vs_out;

bool instanciate = false;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 light_space_matrix;

void main()
{	
  mat4 model1;
  if (instanciate)
    model1 = a_model;
  else 
    model1 = model;
  vec3 pos = position;
  /*float time = u_time + 8.5f * gl_InstanceID;
    vec3 pos = position;
    float fact = 7/position.y;
    float sx = sin(pos.x*32.0+time*4.0)/fact*0.5+0.5;
    float cy = cos(pos.y*32.0+time*4.0)/fact*0.5+0.5;
    vec3 displacement = vec3(sx, cy, sx*cy);
    vec3 normalN = normal_.xyz*2.0 - 1.0;*/

  //pos = pos + normalN*displacement*vec3(0.06, 0.06, 0.06)*vec3(8.0, 3.0, 1.0);

  gl_Position = projection * view * model1 * vec4(pos, 1.0f);
  vs_out.frag_pos = vec3(model1 * vec4(position, 1.0));
  vs_out.normal = transpose(inverse(mat3(model1))) * normal_;
  vs_out.tex_coords = tex_coords_;
  vs_out.frag_pos_light_space = light_space_matrix * vec4(vs_out.frag_pos, 1.0);
}
