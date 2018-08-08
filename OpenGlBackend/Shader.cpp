#include "Shader.h"

#include <glad/glad.h>
#include <iostream>

#include "Model.h"
#include "Matrix.h"

/*
when creating a shader I need :
vertex source
fragment source
atrribArrays [name, data length, data type]
uniforms [name, data type]
*/

#define CHECK_CREATION() \
	if (!this->creationSucceeded)\
		return

#define LOAD_TEMPLATE(TYPE, ARGS...) \
	int location = getUniformLocation(name);\
	glUseProgram(this->programID);\
	glUniform##TYPE(location, ARGS);\
	glUseProgram(0)

#define LOAD_MATRIX(TYPE) \
	LOAD_TEMPLATE(Matrix##TYPE##v, 1, GL_TRUE, data.getGLFormat())

#define LOAD_VECTOR(TYPE) \
	LOAD_TEMPLATE(TYPE##v, 1, data.getGLFormat())

#define LOAD_SINGLE(TYPE) \
	LOAD_TEMPLATE(1##TYPE, data)

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
		for (auto iter = this->attributes.cbegin(); iter != this->attributes.cend(); iter++) 
		{
			Attribute* attr = iter->second;

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
		CHECK_CREATION();

		glUseProgram(this->programID);
	}

	void Shader::unbind() const 
	{
		glUseProgram(0);
	}



	void Shader::addAttribute(const char* name, unsigned int elements, unsigned int offset)
	{
		CHECK_CREATION();

		int location = glGetAttribLocation(this->programID, name);
		addAttribute(location, name, elements, offset);
	}

	void Shader::addAttribute(int location, const char* name, unsigned int elements, unsigned int offset)
	{
		CHECK_CREATION();

		Attribute* attr = new Attribute{ location, elements/*, offset*/ };
		this->attributes.emplace(name, attr);
	}

	const std::unordered_map<std::string, Shader::Attribute*>& Shader::getAttributes() const
	{
		return this->attributes;
	}



	void Shader::setUniform(std::string name, const Mat2 &data)
	{
		CHECK_CREATION();

		LOAD_MATRIX(2f);
	}

	void Shader::setUniform(std::string name, const Mat3 &data)
	{
		CHECK_CREATION();

		LOAD_MATRIX(3f);
	}

	void Shader::setUniform(std::string name, const Mat4 &data)
	{
		CHECK_CREATION();

		LOAD_MATRIX(4f);
	}



	void Shader::setUniform(std::string name, const Vec2 &data)
	{
		CHECK_CREATION();

		LOAD_VECTOR(2f);
	}

	void Shader::setUniform(std::string name, const iVec2 &data)
	{
		CHECK_CREATION();

		LOAD_VECTOR(2i);
	}

	void Shader::setUniform(std::string name, const uVec2 &data)
	{
		CHECK_CREATION();

		LOAD_VECTOR(2ui);
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
		CHECK_CREATION();

		LOAD_VECTOR(3f);
	}

	void Shader::setUniform(std::string name, const iVec3 &data)
	{
		CHECK_CREATION();

		LOAD_VECTOR(3i);
	}

	void Shader::setUniform(std::string name, const uVec3 &data)
	{
		CHECK_CREATION();

		LOAD_VECTOR(3ui);
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
		CHECK_CREATION();

		LOAD_VECTOR(4f);
	}

	void Shader::setUniform(std::string name, const iVec4 &data)
	{
		CHECK_CREATION();

		LOAD_VECTOR(4i);
	}

	void Shader::setUniform(std::string name, const uVec4 &data)
	{
		CHECK_CREATION();

		LOAD_VECTOR(4ui);
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
		CHECK_CREATION();

		LOAD_SINGLE(f);
	}
	
	void Shader::setUniform(std::string name, int data)
	{
		CHECK_CREATION();

		LOAD_SINGLE(i);
	}

	void Shader::setUniform(std::string name, unsigned int data)
	{
		CHECK_CREATION();

		LOAD_SINGLE(ui);
	}

	void Shader::setUniform(std::string name, bool data)
	{
		CHECK_CREATION();

		LOAD_SINGLE(i);
	}

	void Shader::setTexture(int textureSlot, std::string uniformName, int textureId)
	{
		CHECK_CREATION();

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



	// void Shader::draw(std::shared_ptr<Model> model)
	// {
	// 	this->bind();
	// 	model->bindToShader(shared_from_this());

	// 	if (model->getHasEBO())
	// 	{
	// 		glDrawElements(GL_TRIANGLES, model->getDrawCount(), GL_UNSIGNED_INT, 0);
	// 	}
	// 	else
	// 	{
	// 		glDrawArrays(GL_TRIANGLES, 0, model->getDrawCount());
	// 	}

	// 	model->unbind();
	// 	this->unbind();
	// }
}
