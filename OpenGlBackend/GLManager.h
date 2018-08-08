#pragma once

#include <unordered_map>
#include <memory>

#include "MatrixTypes.h"

namespace Resources
{
	enum class Shader : int;
	enum class Model : int;
	enum class Texture : int;
}

namespace GLBackend
{
	class Shader;
	class Model;
}

class GLManager
{
	std::unordered_map<int, std::shared_ptr<GLBackend::Shader>> shaders;
	std::unordered_map<int, std::shared_ptr<GLBackend::Model>> models;
	std::unordered_map<int, unsigned int> textures;

	std::string texturePath = "";

public:
	GLManager();
	~GLManager();

	void enableDebugOutput();

	std::shared_ptr<GLBackend::Shader> createShaderProgram(Resources::Shader shader, unsigned int vertexElements, const char* vertexSource, const char* fragmentSource);
	std::shared_ptr<GLBackend::Shader> getShader(Resources::Shader shader);

	void setTexturePath(std::string path);
	void loadTexture(Resources::Texture tex, std::string fileName);
	int getTexture(Resources::Texture tex);

	std::shared_ptr<GLBackend::Model> addModel(Resources::Model model, float vertices[], unsigned int verticesCount, int numVertexElements);
	std::shared_ptr<GLBackend::Model> addModel(Resources::Model model, float vertices[], unsigned int verticesCount, int numVertexElements, unsigned int EBO[], unsigned int EBOTriangles);
	std::shared_ptr<GLBackend::Model> getModel(Resources::Model model);
};
