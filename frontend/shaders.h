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


const char* textureVertexSource =
"#version 330 core\n\
in vec3 aPos;\n\
in vec2 aTexCoord;\n\
\n\
out vec2 TexCoord;\n\
\n\
uniform mat4 model;\n\
uniform mat4 camera;\n\
uniform mat4 projection;\n\
\n\
void main()\n\
{\n\
gl_Position = projection * camera * model * vec4(aPos, 1.0);\n\
TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);\n\
}";

const char* textureFragmentSource =
"#version 330 core\n\
out vec4 FragColor;\n\
\n\
in vec2 TexCoord;\n\
\n\
uniform sampler2D ourTexture;\n\
\n\
void main()\n\
{\n\
vec4 texel = texture(ourTexture, TexCoord);\n\
if(texel.a < 0.5)\n\
	discard;\n\
FragColor = texel;\n\
}";
