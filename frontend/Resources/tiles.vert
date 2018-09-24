#version 420 core
in vec2 aPos;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;

uniform int ROWS = 8;
uniform int COLUMNS = 16;

void main()
{
	gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
	TexCoord = aPos;
	TexCoord.x /= COLUMNS;
	TexCoord.y = (1 - TexCoord.y) / ROWS;
}