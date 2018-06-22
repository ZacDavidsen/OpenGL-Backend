#pragma once
#include <unordered_map>
#include "MatrixTypes.h"
#include "Shader.h"

namespace GLBackend
{
	class Shader;
	class Model;
}

class GLManager
{
	std::unordered_map<int, GLBackend::Shader*> shaders;
	std::unordered_map<int, GLBackend::Model*> models;
	std::unordered_map<int, unsigned int> textures;

	std::string texturePath = "";

public:
	GLManager();
	~GLManager();

	void createShaderProgram(int referenceId, unsigned int vertexElements, const char* vertexSource, const char* fragmentSource);
	void addShaderAttribute(int shaderId, const char* name, unsigned int elements, unsigned int offset);
	void addShaderAttribute(int shaderId, int location, const char* name, unsigned int elements, unsigned int offset);

	void setTextureFolder(std::string path);
	void loadTexture(int referenceId, std::string fileName);

	template<unsigned int height, unsigned int width, typename T>
	void loadUniform(int shaderId, std::string name, const Mat::Matrix<height, width, T> data);
	template<unsigned int height, typename T>
	void loadUniform(int shaderId, std::string name, const Mat::Vector<height, T> data);

	void setTextureUniform(int shaderId, int textureSlot, std::string uniformName, int textureId);

	void addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[]);
	//void addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[], unsigned int textureId);
	void addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[], unsigned int EBOTriangles, unsigned int EBO[]);
	//void addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[], unsigned int EBOTriangles, unsigned int EBO[], unsigned int textureId);

	void drawItem(int modelId);
	void drawItem(int shaderId, int modelId);
};



template<unsigned int height, unsigned int width, typename T>
void GLManager::loadUniform(int shaderId, std::string name, const Mat::Matrix<height, width, T> data)
{
	shaders.at(shaderId)->setUniform(name, data);
}

template<unsigned int height, typename T>
void GLManager::loadUniform(int shaderId, std::string name, const Mat::Vector<height, T> data)
{
	shaders.at(shaderId)->setUniform(name, data);
}

