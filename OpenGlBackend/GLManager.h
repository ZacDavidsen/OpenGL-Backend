#pragma once
#include <unordered_map>
#include "Matrix.h"

namespace/*class*/ GLManager
{
	//std::unordered_map<int, ::Shader> shaders;
	//std::unordered_map<int, ::Model> models;
	//std::unordered_map<int, unsigned int> textures;

	//std::string texturePath = "";

//public:
//	GLManager();
//	~GLManager();

	void createShaderProgram(int referenceId, unsigned int vertexElements, const char* vertexSource, const char* fragmentSource);
	void addShaderAttribute(int shaderId, const char* name, unsigned int elements, unsigned int offset);
	void addShaderAttribute(int shaderId, int location, const char* name, unsigned int elements, unsigned int offset);

	void setTextureFolder(std::string path);
	void loadTexture(int referenceId, std::string fileName);

	void loadUniform(int shaderId, std::string name, Mat4 data);
	void loadUniform(int shaderId, std::string name, Vec3 data);

	void addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[]);
	void addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[], unsigned int textureId);
	void addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[], unsigned int EBOTriangles, unsigned int EBO[]);
	void addModel(int referenceId, int shaderId, unsigned int verticesCount, float vertices[], unsigned int EBOTriangles, unsigned int EBO[], unsigned int textureId);

	void drawItem(int modelId, int textureId = 0);
};

