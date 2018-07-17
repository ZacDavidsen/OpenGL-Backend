#version 440 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D text;
uniform int character;

uniform int ROWS = 8;
uniform int COLUMNS = 16;

void main()
{
	if(character > 127 || character < 0)
		discard;
	
	vec2 base = vec2(mod(character, COLUMNS), ROWS - floor(character / COLUMNS));
	base.y = 1 - (base.y / ROWS);
	base.x /= COLUMNS;
	
	vec4 texel = texture(text, TexCoord + base);
	if(texel.r < 0.5)
		discard;

	FragColor =  texel;
}