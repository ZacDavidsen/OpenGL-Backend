#version 420 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tiledTexture;
uniform int tile;

uniform int ROWS = 8;
uniform int COLUMNS = 16;

void main()
{
	if(tile > 127 || tile < 0)
		discard;
	
	vec2 base = vec2(mod(tile, COLUMNS), ROWS - floor(tile / COLUMNS));
	base.y = 1 - (base.y / ROWS);
	base.x /= COLUMNS;
	
	vec4 texel = texture(tiledTexture, TexCoord + base);
	if(texel.r < 0.5)
		discard;

	FragColor =  texel;
}