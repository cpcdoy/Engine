#version 330 core

layout(location=0) in vec3 position_;
//layout(location=2) in vec3 normal;

uniform mat4 VP;
uniform mat4 model;

//uniform float u_time;

void main()
{
	/*float time = u_time + 8.5f * gl_InstanceID;
	vec3 pos = position_;
	float fact = 7/position_.y;
	float sx = sin(pos.x*32.0+time*4.0)/fact*0.5+0.5;
	float cy = cos(pos.y*32.0+time*4.0)/fact*0.5+0.5;
	vec3 displacement = vec3(sx, cy, sx*cy);
	vec3 normalN = normal.xyz*2.0 - 1.0;*/

	//pos = pos;// + normalN*displacement*vec3(0.06, 0.06, 0.06)*vec3(8.0, 3.0, 1.0);

	gl_Position = VP * model * vec4(position_, 1.0f);
}
