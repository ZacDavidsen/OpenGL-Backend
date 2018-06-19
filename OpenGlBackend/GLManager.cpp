#include "GLManager.h"
#include <glad\glad.h>
#include <iostream>

#include "Shader.h"
#include "Model.h"
#include "Matrix.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/*

std::unordered_map<int, GLBackend::Shader> shaders;
std::unordered_map<int, GLBackend::Model> models;
std::unordered_map<int, unsigned int> textures;

std::string texturePath = "";

/*/

GLManager::GLManager()
{
	shaders = std::unordered_map<int, GLBackend::Shader*>();
	models = std::unordered_map<int, GLBackend::Model*>();
	textures = std::unordered_map<int, unsigned int>();
}

GLManager::~GLManager()
{
	for (auto iter = shaders.cbegin(); iter != shaders.cend(); iter++) 
	{
		GLBackend::Shader* pnt = iter->second;

		delete pnt;
	}
	this->shaders.clear();

	for (auto iter = models.cbegin(); iter != models.cend(); iter++)
	{
		GLBackend::Model* pnt = iter->second;

		delete pnt;
	}
	this->models.clear();
}

//*/

void GLManager::createShaderProgram(int referenceId, unsigned int vertexElements, const char* vertexSource, const char* fragmentSource)
{
	GLBackend::Shader* shader = new GLBackend::Shader(vertexElements, vertexSource, fragmentSource);
	shaders.emplace(referenceId, shader);
	//return shaders.at(referenceId);
}

void GLManager::addShaderAttribute(int shaderId, const char* name, unsigned int elements, unsigned int offset)
{
	shaders.at(shaderId)->addAttribute(name, elements, offset);
}

void GLManager::addShaderAttribute(int shaderId, int location, const char* name, unsigned int elements, unsigned int offset)
{
	shaders.at(shaderId)->addAttribute(location, name, elements, offset);
}


void GLManager::loadUniform(int shaderId, std::string name, const Mat4 data)
{
	shaders.at(shaderId)->loadUniform(name, data);
}

void GLManager::loadUniform(int shaderId, std::string name, const Vec3 data)
{
	shaders.at(shaderId)->loadUniform(name, data);
}

void GLManager::setTextureUniform(int shaderId, int textureSlot, std::string uniformName, int textureId)
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, textures.at(textureId));
	shaders.at(shaderId)->loadUniform(uniformName, textureSlot);
	glActiveTexture(textureSlot);
}


void GLManager::setTextureFolder(std::string path)
{
	/*this->*/texturePath = path;
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

	unsigned char *data = stbi_load((/*this->*/texturePath + fileName).c_str(), &width, &height, &origBitsPerPixel, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	textures.emplace(referenceId, texture);

	glBindTexture(GL_TEXTURE_2D, 0);
}



void GLManager::addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[])
{
	GLBackend::Shader* shader = shaders.at(shaderId);
	unsigned int numElements = shader->getVertexElements();

	//TODO refactor for color suppport
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numElements * verticesCount, vertices, GL_STATIC_DRAW);

	for (auto attr : shader->getAttributes())
	{
		glVertexAttribPointer(attr->location, attr->size, GL_FLOAT, GL_FALSE, numElements * sizeof(float), (void*)(attr->offset * sizeof(float)));
		glEnableVertexAttribArray(attr->location);
	}

	GLBackend::Model* model = new GLBackend::Model(shader->getId(), VAO, verticesCount);
	models.emplace(referenceId, model);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//void GLManager::addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[], unsigned int textureId)
//{
//	//TODO refactor for color suppport
//	unsigned int VAO;
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	unsigned int VBO;
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * verticesCount, vertices, GL_STATIC_DRAW);
//
//	Shader& shader = shaders.at(shaderId);
//	for (auto attr : shader.getAttributes())
//	{
//		glVertexAttribPointer(attr.location, attr.size, GL_FLOAT, GL_FALSE, shader.getVertexElements() * sizeof(float), (void*)(attr.offset * sizeof(float)));
//		glEnableVertexAttribArray(attr.location);
//	}
//
//	Model model(shader, VAO, verticesCount);
//	models.emplace(referenceId, model);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//}

void GLManager::addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[], unsigned int EBOTriangles, unsigned int EBO[])
{
	GLBackend::Shader* shader = shaders.at(shaderId);
	unsigned int numElements = shader->getVertexElements();

	//TODO refactor for color suppport
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numElements * verticesCount, vertices, GL_STATIC_DRAW);

	unsigned int EBOid;
	glGenBuffers(1, &EBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * EBOTriangles, EBO, GL_STATIC_DRAW);	

	for (auto attr : shader->getAttributes())
	{
		glVertexAttribPointer(attr->location, attr->size, GL_FLOAT, GL_FALSE, numElements * sizeof(float), (void*)(attr->offset * sizeof(float)));
		glEnableVertexAttribArray(attr->location);
	}

	GLBackend::Model* model = new GLBackend::Model(shader->getId(), VAO, EBOTriangles * 3, true);
	models.emplace(referenceId, model);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//void GLManager::addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[], unsigned int EBOTriangles, unsigned int EBO[], unsigned int textureId)
//{
//	Shader& shader = shaders.at(shaderId);
//	unsigned int numElements = shader.getVertexElements();
//
//	//TODO refactor for color suppport
//	unsigned int VAO;
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	unsigned int VBO;
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numElements * verticesCount, vertices, GL_STATIC_DRAW);
//
//	unsigned int EBOid;
//	glGenBuffers(1, &EBOid);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * EBOTriangles, EBO, GL_STATIC_DRAW);
//
//	for (auto attr : shader.getAttributes())
//	{
//		glVertexAttribPointer(attr.location, attr.size, GL_FLOAT, GL_FALSE, numElements * sizeof(float), (void*)(attr.offset * sizeof(float)));
//		glEnableVertexAttribArray(attr.location);
//	}
//
//	Model model(shader, VAO, EBOTriangles * 3, true);
//	models.emplace(referenceId, model);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//}



void GLManager::drawItem(int modelId)
{
	models.at(modelId)->drawModel();
}


void GLManager::drawItem(int shaderId, int modelId) {
	shaders.at(shaderId)->bind();
	GLBackend::Model* model = models.at(modelId);
	glBindVertexArray(model->VAO);

	if (model->hasEBO)
	{
		glDrawElements(GL_TRIANGLES, model->drawCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, model->drawCount);
	}

	glBindVertexArray(0);
	glUseProgram(0);
}
