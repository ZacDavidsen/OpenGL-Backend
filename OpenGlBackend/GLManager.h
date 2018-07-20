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

	void enableDebugOutput();

	void createShaderProgram(int referenceId, unsigned int vertexElements, const char* vertexSource, const char* fragmentSource);
	void addShaderAttribute(int shaderId, const char* name, unsigned int elements, unsigned int offset);
	void addShaderAttribute(int shaderId, int location, const char* name, unsigned int elements, unsigned int offset);

	void setTexturePath(std::string path);
	void loadTexture(int referenceId, std::string fileName);

	template<unsigned int height, unsigned int width, typename T>
	void loadUniform(int shaderId, std::string name, const Mat::Matrix<height, width, T> data);
	template<unsigned int height, typename T>
	void loadUniform(int shaderId, std::string name, const Mat::Vector<height, T> data);
	void loadUniform(int shaderId, std::string name, int value);
	void loadUniform(int shaderId, std::string name, unsigned int value);

	void setTextureUniform(int shaderId, int textureSlot, std::string uniformName, int textureId);

	void addModel(int referenceId, float vertices[], unsigned int verticesCount, int numVertexElements);
	void addModel(int referenceId, float vertices[], unsigned int verticesCount, int numVertexElements, unsigned int EBO[], unsigned int EBOTriangles);

	void drawItem(int shaderId, int modelId);
};


//Don't really like having these here, but the template makes it so easy..
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

