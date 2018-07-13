#pragma once

namespace GLBackend
{
	class Shader;

	class Model
	{
		unsigned int lastBoundShader = 0;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		unsigned int drawCount;
		bool hasEBO;

	public:

		Model(float *vertices, unsigned int numVertices, unsigned int numVertexElements);
		Model(float *vertices, unsigned int numVertices, unsigned int numVertexElements, unsigned int *EBO, unsigned int numTriangles);
		~Model();

		bool getHasEBO() const;
		unsigned int getDrawCount() const;

		void bindToShader(Shader const *shader);
		void unbind() const;
	};
}

