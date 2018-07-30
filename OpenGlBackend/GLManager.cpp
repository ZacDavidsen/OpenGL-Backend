#include "GLManager.h"
#include <glad/glad.h>
#include <iostream>
#include <map>

#include "Shader.h"
#include "Model.h"
#include "Matrix.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void* userParam);



GLManager::GLManager()
{
	
}

GLManager::~GLManager()
{
	for (auto entry : this->shaders)
	{
		GLBackend::Shader* shader = entry.second;

		delete shader;
	}
	this->shaders.clear();

	for (auto entry : this->models)
	{
		GLBackend::Model* model = entry.second;

		delete model;
	}
	this->models.clear();

	unsigned int *texs = new unsigned int[this->textures.size()];
	int i = 0;
	for (auto entry : this->textures)
	{
		texs[i++] = entry.second;
	}
	glDeleteTextures(textures.size(), texs);
	delete[] texs;
	this->textures.clear();

}



void GLManager::enableDebugOutput()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(openglDebugCallback, 0);
}



void GLManager::createShaderProgram(int referenceId, unsigned int vertexElements, const char* vertexSource, const char* fragmentSource)
{
	GLBackend::Shader* shader = new GLBackend::Shader(vertexElements, vertexSource, fragmentSource);
	if (!shader->isGood())
	{
		std::cout << "------- SHADER CREATION FAILED -------" << std::endl;
		std::cout << shader->getInfoLog() << std::endl;
		std::cout << "--------------------------------------" << std::endl;
		//throw std::runtime_error("SHADER CREATION FAILED");
	}
	this->shaders.emplace(referenceId, shader);
}

void GLManager::addShaderAttribute(int shaderId, const char* name, unsigned int elements, unsigned int offset)
{
	this->shaders.at(shaderId)->addAttribute(name, elements, offset);
}

void GLManager::addShaderAttribute(int shaderId, int location, const char* name, unsigned int elements, unsigned int offset)
{
	this->shaders.at(shaderId)->addAttribute(location, name, elements, offset);
}



void GLManager::loadUniform(int shaderId, std::string name, int value)
{
	this->shaders.at(shaderId)->setUniform(name, value);
}

void GLManager::loadUniform(int shaderId, std::string name, unsigned int value)
{
	this->shaders.at(shaderId)->setUniform(name, value);
}

void GLManager::setTextureUniform(int shaderId, int textureSlot, std::string uniformName, int textureId)
{
	this->shaders.at(shaderId)->setTexture(textureSlot, uniformName, textures.at(textureId));
}

void GLManager::setTexturePath(std::string path)
{
	this->texturePath = path;
}

void GLManager::loadTexture(int referenceId, std::string fileName)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	// width and height are in pixels
	int width, height, origBitsPerPixel;

	unsigned char *data = stbi_load((this->texturePath + fileName).c_str(), &width, &height, &origBitsPerPixel, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	this->textures.emplace(referenceId, texture);

	glBindTexture(GL_TEXTURE_2D, 0);
}



void GLManager::addModel(int referenceId, float vertices[], unsigned int verticesCount, int numVertexElements)
{
	GLBackend::Model* model = new GLBackend::Model(vertices, verticesCount, numVertexElements);
	this->models.emplace(referenceId, model);
}

void GLManager::addModel(int referenceId, float vertices[], unsigned int verticesCount, int numVertexElements, unsigned int EBO[], unsigned int EBOTriangles)
{
	GLBackend::Model* model = new GLBackend::Model(vertices, verticesCount, numVertexElements, EBO, EBOTriangles);
	this->models.emplace(referenceId, model);
}



void GLManager::addModelAttribute(int modelId, std::string name, int size, int offset) 
{
	this->models.at(modelId)->addAttribute(name, size, offset);
}



void GLManager::drawItem(int shaderId, int modelId) {
	GLBackend::Shader* shader = this->shaders.at(shaderId);
	GLBackend::Model* model = this->models.at(modelId);

	shader->bind();
	model->bindToShader(shader);

	if (model->getHasEBO())
	{
		glDrawElements(GL_TRIANGLES, model->getDrawCount(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, model->getDrawCount());
	}

	model->unbind();
	shader->unbind();
}



//Would be kinda nice to not have these file scope
std::map<GLenum, const char*> sourceEnum{
	{ GL_DEBUG_SOURCE_API,				"API" },
	{ GL_DEBUG_SOURCE_WINDOW_SYSTEM,	"WINDOW SYSTEM" },
	{ GL_DEBUG_SOURCE_SHADER_COMPILER,	"SHADER COMPILER" },
	{ GL_DEBUG_SOURCE_THIRD_PARTY,		"THIRD PARTY" },
	{ GL_DEBUG_SOURCE_APPLICATION,		"APPLICATION" },
	{ GL_DEBUG_SOURCE_OTHER,			"OTHER" }
};

std::map<GLenum, const char*> typeEnum{
	{ GL_DEBUG_TYPE_ERROR,				"ERROR" },
	{ GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,"DEPRECATED BEHAVIOR" },
	{ GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,	"UNDEFINED BEHAVIOR" },
	{ GL_DEBUG_TYPE_PORTABILITY,		"PORTABILITY" },
	{ GL_DEBUG_TYPE_PERFORMANCE,		"PERFORMANCE" },
	{ GL_DEBUG_TYPE_MARKER,				"MARKER" },
	{ GL_DEBUG_TYPE_PUSH_GROUP,			"PUSH GROUP" },
	{ GL_DEBUG_TYPE_POP_GROUP,			"POP GROUP" },
	{ GL_DEBUG_TYPE_OTHER,				"OTHER" },
};

std::map<GLenum, const char*> severityEnum{
	{ GL_DEBUG_SEVERITY_HIGH,			"HIGH" },
	{ GL_DEBUG_SEVERITY_MEDIUM,			"MEDIUM" },
	{ GL_DEBUG_SEVERITY_LOW,			"LOW" },
	{ GL_DEBUG_SEVERITY_NOTIFICATION,	"NOTIFICATION" },
};

void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "---------- GL DEBUG MESSAGE ----------" << std::endl;
	std::cout << "Source:   " << sourceEnum[source] << std::endl;
	std::cout << "Type:     " << typeEnum[type] << std::endl;
	std::cout << "ID:       " << id << std::endl;
	std::cout << "Severity: " << severityEnum[severity] << std::endl;
	std::cout << "Message:\n" << message << std::endl;
	std::cout << "--------------------------------------" << std::endl;
}
