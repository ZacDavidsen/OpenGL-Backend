#include "Shader.h"
#include "Matrix.h"
#include <glad\glad.h>
#include <iostream>

/*
when creating a shader I need :
vertex source
fragment source
atrribArrays [name, data length, data type]
uniforms [name, data type]
*/

namespace GLBackend
{
	Shader::Shader(unsigned int vertexElements, const char* vertexSource, const char* fragmentSource)
	{
		//TODO break shader stages into their own class, for reuse, better management
		int success;

		this->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(this->vertexShaderID, 1, &vertexSource, NULL);
		glCompileShader(this->vertexShaderID);
		glGetShaderiv(this->vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int logLength;
			glGetShaderiv(this->vertexShaderID, GL_INFO_LOG_LENGTH, &logLength);
			this->infoLog = new char[logLength];

			glGetShaderInfoLog(this->vertexShaderID, logLength, NULL, this->infoLog);
			this->creationSucceeded = false;
			return;
		}

		this->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(this->fragmentShaderID, 1, &fragmentSource, NULL);
		glCompileShader(this->fragmentShaderID);
		glGetShaderiv(this->fragmentShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int logLength;
			glGetShaderiv(this->fragmentShaderID, GL_INFO_LOG_LENGTH, &logLength);
			this->infoLog = new char[logLength];

			glGetShaderInfoLog(this->fragmentShaderID, 512, NULL, this->infoLog);
			this->creationSucceeded = false;
			return;
		}

		this->programID = glCreateProgram();
		glAttachShader(this->programID, this->vertexShaderID);
		glAttachShader(this->programID, this->fragmentShaderID);
		glLinkProgram(this->programID);
		glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			int logLength;
			glGetShaderiv(this->programID, GL_INFO_LOG_LENGTH, &logLength);
			this->infoLog = new char[logLength];

			glGetProgramInfoLog(this->programID, 512, NULL, this->infoLog);
			this->creationSucceeded = false;
			return;
		}

		this->vertexElements = vertexElements;
		this->creationSucceeded = true;
	}

	Shader::~Shader()
	{
		//TODO: This destructor might cause problems if the shader is ever passed by value....
		for (auto iter = this->attributes.crbegin(); iter != this->attributes.crend(); iter++) 
		{
			Attribute* attr = *iter;

			delete attr;
		}
		this->attributes.clear();

		for (auto iter = this->uniforms.cbegin(); iter != this->uniforms.cend(); iter++)
		{
			Uniform* uni = iter->second;

			delete uni;
		}
		this->uniforms.clear();

		if (this->infoLog != nullptr)
		{
			delete[] this->infoLog;
		}

		glDeleteShader(this->vertexShaderID);
		glDeleteShader(this->fragmentShaderID);
		glDeleteProgram(this->programID);
	}



	void Shader::bind() const 
	{
		if (!this->creationSucceeded)
			return;

		glUseProgram(this->programID);
	}

	void Shader::unbind() const 
	{
		glUseProgram(0);
	}



	void Shader::addAttribute(const char* name, unsigned int elements, unsigned int offset)
	{
		if (!this->creationSucceeded)
			return;

		int location = glGetAttribLocation(this->programID, name);
		addAttribute(location, name, elements, offset);
	}

	void Shader::addAttribute(int location, const char* name, unsigned int elements, unsigned int offset)
	{
		if (!this->creationSucceeded)
			return;

		Attribute* attr = new Attribute{ location, elements, offset };
		this->attributes.push_back(attr);
	}

	const std::vector<Attribute*>& Shader::getAttributes() const
	{
		return this->attributes;
	}



	void Shader::setUniform(std::string name, const Mat2 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniformMatrix2fv(location, 1, GL_TRUE, data.getGLFormat());
		glUseProgram(0);
	}

	void Shader::setUniform(std::string name, const Mat3 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniformMatrix3fv(location, 1, GL_TRUE, data.getGLFormat());
		glUseProgram(0);
	}

	void Shader::setUniform(std::string name, const Mat4 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniformMatrix4fv(location, 1, GL_TRUE, data.getGLFormat());
		glUseProgram(0);
	}



	void Shader::setUniform(std::string name, const Vec2 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform2fv(location, 1, data.getGLFormat());
		glUseProgram(0);
	}

	void Shader::setUniform(std::string name, const iVec2 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform2iv(location, 1, data.getGLFormat());
		glUseProgram(0);
	}

	void Shader::setUniform(std::string name, const uVec2 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform2uiv(location, 1, data.getGLFormat());
		glUseProgram(0);
	}

	//void Shader::setUniform(std::string name, const bVec2 data)
	//{
	//	int location = getUniformLocation(name);

	//	glUseProgram(this->programID);
	//	glUniform2iv(location, 1, data.getGLFormat());
	//	glUseProgram(0);
	//}



	void Shader::setUniform(std::string name, const Vec3 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform3fv(location, 1, data.getGLFormat());
		glUseProgram(0);
	}

	void Shader::setUniform(std::string name, const iVec3 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform3iv(location, 1, data.getGLFormat());
		glUseProgram(0);
	}

	void Shader::setUniform(std::string name, const uVec3 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform3uiv(location, 1, data.getGLFormat());
		glUseProgram(0);
	}

	//void Shader::setUniform(std::string name, const bVec3 data)
	//{
	//	int location = getUniformLocation(name);

	//	glUseProgram(this->programID);
	//	glUniform3iv(location, 1, data.getGLFormat());
	//	glUseProgram(0);
	//}



	void Shader::setUniform(std::string name, const Vec4 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform4fv(location, 1, data.getGLFormat());
		glUseProgram(0);
	}

	void Shader::setUniform(std::string name, const iVec4 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform4iv(location, 1, data.getGLFormat());
		glUseProgram(0);
	}

	void Shader::setUniform(std::string name, const uVec4 &data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform4uiv(location, 1, data.getGLFormat());
		glUseProgram(0);
	}

	//void Shader::setUniform(std::string name, const bVec4 data)
	//{
	//	int location = getUniformLocation(name);

	//	glUseProgram(this->programID);
	//	glUniform4iv(location, 1, data.getGLFormat());
	//	glUseProgram(0);
	//}



	void Shader::setUniform(std::string name, float data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform1f(location, data);
		glUseProgram(0);
	}
	
	void Shader::setUniform(std::string name, int data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform1i(location, data);
		glUseProgram(0);
	}

	void Shader::setUniform(std::string name, unsigned int data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform1ui(location, data);
		glUseProgram(0);
	}

	void Shader::setUniform(std::string name, bool data)
	{
		if (!this->creationSucceeded)
			return;

		int location = getUniformLocation(name);

		glUseProgram(this->programID);
		glUniform1i(location, data);
		glUseProgram(0);
	}

	void Shader::setTexture(int textureSlot, std::string uniformName, int textureId)
	{
		if (!this->creationSucceeded)
			return;

		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, textureId);
		setUniform(uniformName, textureSlot);
	}



	int Shader::getUniformLocation(std::string name) 
	{
		Uniform* uni;
		auto locationIterator = this->uniforms.find(name);
		if (locationIterator != this->uniforms.end())
		{
			uni = locationIterator->second;
		}
		else
		{//then we dont know the location, so we need to find it
			int location = glGetUniformLocation(this->programID, name.c_str());
			uni = new Uniform{ location };
			this->uniforms.emplace(name, uni);
		}
		return uni->location;
	}
}
