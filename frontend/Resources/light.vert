#version 420 core
in vec3 aPos;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	gl_Position = projection * camera * model * vec4(aPos, 1.0);
}