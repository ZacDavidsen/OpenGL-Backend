#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace GLBackend
{
	class Shader;

	class Model
	{
		struct Attribute;

		unsigned int lastBoundShader = 0;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		unsigned int drawCount;
		bool hasEBO;

		std::unordered_map<std::string, Attribute*> attributes;

	public:

		Model(float *vertices, unsigned int numVertices, unsigned int elementsPerVertex);
		Model(float *vertices, unsigned int numVertices, unsigned int elementsPerVertex, unsigned int *EBO, unsigned int numTriangles);
		~Model();

		bool getHasEBO() const;
		unsigned int getDrawCount() const;

		void addAttribute(std::string name, int size, int offset);

		void drawWithShader(std::shared_ptr<Shader> shader);
		
	private:
		void bindToShader(const std::shared_ptr<Shader> shader);
		void unbind() const;
	};

	struct Model::Attribute {
		//int VBO;
		int size;
		int offset;
	};
}

