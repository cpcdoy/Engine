#version 330 core
in vec3 position;
in vec2 tex_coords_;

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
uniform mat4 model;
uniform mat3 trans_inv_model;

// Uffizi gallery
/*const vec3 L00  = vec3(0.32, 0.31, 0.35);
const vec3 L1m1 = vec3(0.37, 0.37, 0.43);
const vec3 L10  = vec3(0.0, 0.0, 0.0);
const vec3 L11  = vec3(0.1,0.1,0.1);
const vec3 L2m2 = vec3(0.02,0.02,0.03);
const vec3 L2m1 = vec3(0.01,0.01,0.01);
const vec3 L20  = vec3(0.28,0.28,0.32);
const vec3 L21  = vec3(0.0,0.0,0.0);
const vec3 L22  = vec3(0.24,0.24,0.28);*/
// Old Town Square
/*const vec3 L00  = vec3( 0.871297,  0.875222,  0.864470 );
const vec3 L1m1 = vec3( 0.175058,  0.245335,  0.312891 );
const vec3 L10  = vec3( 0.034675,  0.036107,  0.037362 );
const vec3 L11  = vec3(-0.004629, -0.029448, -0.048028);
const vec3 L2m2 = vec3(-0.120535, -0.121160, -0.117507);
const vec3 L2m1 = vec3( 0.003242,  0.003624,  0.007511 );
const vec3 L20  = vec3(-0.028667, -0.024926, -0.020998);
const vec3 L21  = vec3(-0.077539, -0.086325, -0.091591);
const vec3 L22  = vec3(-0.161784, -0.191783, -0.219152);*/

void main()
{
  /*float time = u_time + 8.5f * gl_InstanceID;
    vec3 pos = position;
    float fact = 7/position.y;
    float sx = sin(pos.x*32.0+time*4.0)/fact*0.5+0.5;
    float cy = cos(pos.y*32.0+time*4.0)/fact*0.5+0.5;
    vec3 displacement = vec3(sx, cy, sx*cy);
    vec3 normalN = vs_out.normal.xyz*2.0 - 1.0;*/

  //pos = pos + normalN*displacement*vec3(0.06, 0.06, 0.06)*vec3(8.0, 3.0, 1.0);

  gl_Position = vec4(position, 1.0f);
  /*vs_out.frag_pos = vec3(model * vec4(position, 1.0));
  vs_out.normal = trans_inv_model * normal_;*/
  vs_out.tex_coords = tex_coords_;
  //vs_out.frag_pos_light_space = light_space_matrix * vec4(vs_out.frag_pos, 1.0);
}
