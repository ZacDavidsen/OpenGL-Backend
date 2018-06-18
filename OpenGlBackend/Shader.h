#pragma once
#include <vector>
#include <unordered_map>
#include "MatrixTypes.h"

namespace GLBackend
{
	struct Attribute;
	struct Uniform;

	class Shader
	{
		unsigned int id;
		unsigned int vertexElements;

		std::vector<Attribute*> attributes;
		std::unordered_map<std::string, Uniform*> uniforms;

		int getUniformLocation(std::string name);

	public:
		//Shader();
		Shader(unsigned int vertexElements, const char* vertexSource, const char* fragmentSource);
		~Shader();

		void bind() const;

		unsigned int getId() const { return this->id; };
		unsigned int getVertexElements() const { return this->vertexElements; };

		void addAttribute(const char* name, unsigned int elements, unsigned int offset);
		void addAttribute(int location, const char* name, unsigned int elements, unsigned int offset);
		const std::vector<Attribute*>& getAttributes() const;

		void loadUniform(std::string name, const Mat4 data);
		void loadUniform(std::string name, const Vec3 data);
		void loadUniform(std::string name, int data);
	};

	struct Attribute
	{
		int location;
		unsigned int size;
		//dataType
		unsigned int offset;
	};

	struct Uniform
	{
		int location;
		//unsigned int dataType;
	};
}
