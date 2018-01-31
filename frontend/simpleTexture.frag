#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 lightCol;
in vec3 FragPos;
in vec3 norm;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

void main()
{
	vec4 texel = texture(ourTexture, TexCoord);
	if(texel.a < 0.5)
		discard;

	float specStrength = 0.5;

	vec3 viewPos = cameraPos;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(normalize(FragPos - lightPos), norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specStrength * spec * lightColor;

	FragColor = texel * vec4((lightCol + specular),1);
}