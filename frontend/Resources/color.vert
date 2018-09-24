#version 420 core
in vec3 aPos;
in vec3 aColor;
out vec3 color;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	gl_Position = projection * camera * model * vec4(aPos, 1.0);
	color=aColor;
}