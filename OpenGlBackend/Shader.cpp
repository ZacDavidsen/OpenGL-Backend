#include "Shader.h"
#include <glad\glad.h>
#include <iostream>

/*
when creating a shader I need :
vertex source
fragment source
atrribArrays [name, data length, data type]
uniforms [name, data type]
*/

//Shader::Shader()
//{
//}

Shader::Shader(unsigned int vertexElements, const char* vertexSource, const char* fragmentSource)
{
	int success;
	char infoLog[512];

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	this->id = shaderProgram;
	this->vertexElements = vertexElements;
	this->attributes = std::vector<AttributeArray>();
}

Shader::~Shader()
{
}

void Shader::addAttributeArray(const char* name, unsigned int elements, unsigned int offset)
{
	int location = glGetAttribLocation(this->id, name);
	this->attributes.push_back(AttributeArray{ location, elements, offset });
}

void Shader::addAttributeArray(int location, const char* name, unsigned int elements, unsigned int offset)
{
	this->attributes.push_back(AttributeArray{ location, elements, offset });
}

void Shader::loadUniform(std::string name, const Mat4 data)
{
	Uniform uni;
	auto locationIterator = this->uniforms.find(name);
	if (locationIterator == this->uniforms.end())
	{//then we dont know the location, so we need to find it
		int location = glGetUniformLocation(this->id, name.c_str());
		uni = Uniform{ location };
		this->uniforms.emplace(name, uni);
	}
	else
	{
		uni = locationIterator->second;
	}

	glUseProgram(this->id);
	glUniformMatrix4fv(uni.location, 1, GL_TRUE, data);
	glUseProgram(0);
}

void Shader::loadUniform(std::string name, const Vec3 data)
{
	Uniform uni;
	auto locationIterator = this->uniforms.find(name);
	if (locationIterator == this->uniforms.end())
	{//then we dont know the location, so we need to find it
		int location = glGetUniformLocation(this->id, name.c_str());
		uni = Uniform{ location };
		this->uniforms.emplace(name, uni);
	}
	else
	{
		uni = locationIterator->second;
	}

	glUseProgram(this->id);
	glUniform3fv(uni.location, 1, data);
	glUseProgram(0);
}

const std::vector<AttributeArray>& Shader::getAttributes() const
{
	return this->attributes;
}
