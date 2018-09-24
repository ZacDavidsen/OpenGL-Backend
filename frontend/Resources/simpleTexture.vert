#version 420 core
in vec3 aPos;
in vec2 aTexCoord;
in vec3 aNorm;

out vec2 TexCoord;
out vec3 lightBase;
out vec3 FragPos;
out vec3 norm;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	gl_Position = projection * camera * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
	vec4 translate = model * vec4(0,0,0,1);
	vec3 Normal = vec3(model * vec4(aNorm, 1.0) - translate);

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * clamp(lightColor,0,1);

	norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * clamp(lightColor,0,1);

	lightBase = (ambient + diffuse);
}