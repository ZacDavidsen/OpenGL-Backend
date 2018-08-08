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
	this->shaders.clear();

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



std::shared_ptr<GLBackend::Shader> GLManager::createShaderProgram(Resources::Shader shader, unsigned int vertexElements, const char* vertexSource, const char* fragmentSource)
{
	std::shared_ptr<GLBackend::Shader> shaderobj = std::make_shared<GLBackend::Shader>(vertexElements, vertexSource, fragmentSource);
	if (!shaderobj->isGood())
	{
		std::cout << "------- SHADER CREATION FAILED -------" << std::endl;
		std::cout << shaderobj
		->getInfoLog() << std::endl;
		std::cout << "--------------------------------------" << std::endl;
		//throw std::runtime_error("SHADER CREATION FAILED");
	}
	this->shaders.emplace(static_cast<int>(shader), shaderobj);
	return shaderobj;
}

std::shared_ptr<GLBackend::Shader> GLManager::getShader(Resources::Shader shader)
{
	//return this->shaders.at(shader);
	return this->shaders.at(static_cast<int>(shader));
}



void GLManager::setTexturePath(std::string path)
{
	this->texturePath = path;
}

void GLManager::loadTexture(Resources::Texture tex, std::string fileName)
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

	this->textures.emplace(static_cast<int>(tex), texture);

	glBindTexture(GL_TEXTURE_2D, 0);
}

int GLManager::getTexture(Resources::Texture tex)
{
	return this->textures.at(static_cast<int>(tex));
}



std::shared_ptr<GLBackend::Model> GLManager::addModel(Resources::Model model, float vertices[], unsigned int verticesCount, int numVertexElements)
{
	std::shared_ptr<GLBackend::Model> modelobj = std::make_shared<GLBackend::Model>(vertices, verticesCount, numVertexElements);
	this->models.emplace(static_cast<int>(model), modelobj);
	return modelobj;
}

std::shared_ptr<GLBackend::Model> GLManager::addModel(Resources::Model model, float vertices[], unsigned int verticesCount, int numVertexElements, unsigned int EBO[], unsigned int EBOTriangles)
{
	std::shared_ptr<GLBackend::Model> modelobj = std::make_shared<GLBackend::Model>(vertices, verticesCount, numVertexElements, EBO, EBOTriangles);
	this->models.emplace(static_cast<int>(model), modelobj);
	return modelobj;
}

std::shared_ptr<GLBackend::Model> GLManager::getModel(Resources::Model model)
{
	return this->models.at(static_cast<int>(model));
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
