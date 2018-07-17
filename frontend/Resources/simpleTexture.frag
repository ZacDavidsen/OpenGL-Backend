#version 440 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 lightBase;
in vec3 FragPos;
in vec3 norm;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

void main()
{
	vec4 texel = texture(ourTexture, TexCoord);
	//hacky way to take care of full transparency without sorting everything
	if(texel.a < 0.5)
		discard;

	float specStrength = 0.5;

	vec3 viewPos = cameraPos;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(normalize(FragPos - lightPos), norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specStrength * spec * clamp(lightColor, 0, 1);

	FragColor = texel * vec4((lightBase + specular),1);
}