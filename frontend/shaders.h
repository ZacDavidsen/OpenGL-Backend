#pragma once
//#include "glad\glad.h"
//#include <string>


const char* colorVertexSource = 
"#version 330 core\n\
in vec3 aPos;\n\
in vec3 aColor;\n\
out vec3 color;\n\
\n\
uniform mat4 model;\n\
uniform mat4 camera;\n\
uniform mat4 projection;\n\
\n\
void main()\n\
{\n\
	gl_Position = projection * camera * model * vec4(aPos, 1.0);\n\
	color=aColor;\n\
}";

const char* colorFragmentSource = 
"#version 330 core\n\
in vec3 color;\n\
out vec4 FragColor;\n\
\n\
void main()\n\
{\n\
	FragColor = vec4(color, 1.0f);\n\
}";


const char* lightVertexSource =
"#version 330 core\n\
in vec3 aPos;\n\
\n\
uniform mat4 model;\n\
uniform mat4 camera;\n\
uniform mat4 projection;\n\
\n\
void main()\n\
{\n\
	gl_Position = projection * camera * model * vec4(aPos, 1.0);\n\
}";

const char* lightFragmentSource =
"#version 330 core\n\
out vec4 FragColor;\n\
\n\
uniform vec3 lightColor;\n\
\n\
void main()\n\
{\n\
	FragColor = vec4(lightColor, 1.0f);\n\
}";


const char* textureVertexSource =
"#version 330 core\n\
in vec3 aPos;\n\
in vec2 aTexCoord;\n\
in vec3 aNorm;\n\
\n\
out vec2 TexCoord;\n\
out vec3 FragPos;\n\
out vec3 Normal;\n\
\n\
uniform mat4 model;\n\
uniform mat4 camera;\n\
uniform mat4 projection;\n\
\n\
void main()\n\
{\n\
	gl_Position = projection * camera * model * vec4(aPos, 1.0);\n\
	FragPos = vec3(model * vec4(aPos, 1.0));\n\
	TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);\n\
	Normal = vec3(model * vec4(aNorm, 1.0));\n\
}";

const char* textureFragmentSource =
"#version 330 core\n\
out vec4 FragColor;\n\
\n\
in vec2 TexCoord;\n\
in vec3 FragPos;\n\
in vec3 Normal;\n\
\n\
uniform sampler2D ourTexture;\n\
uniform vec3 lightColor;\n\
uniform vec3 lightPos;\n\
\n\
void main()\n\
{\n\
	vec4 texel = texture(ourTexture, TexCoord);\n\
	if(texel.a < 0.5)\n\
		discard;\n\
\n\
	float ambientStrength = 0.2;\n\
	vec3 ambient = ambientStrength * lightColor;\n\
\n\
	vec3 norm = normalize(Normal);\n\
	vec3 lightDir = normalize(lightPos - FragPos);\n\
	float diff = max(dot(norm, lightDir), 0.0);\n\
	vec3 diffuse = diff * lightColor;\n\
\n\
	vec3 result = ambient + diffuse;\n\
	FragColor = texel * vec4(result, 1.0);\n\
}";
