#pragma once
#include <vector>
#include <unordered_map>
#include "MatrixForwardDec.h"

struct AttributeArray;
struct Uniform;

class Shader
{
	unsigned int id;
	unsigned int vertexElements;

	std::vector<AttributeArray> attributes;
	std::unordered_map<std::string, Uniform> uniforms;

public:
	//Shader();
	Shader(unsigned int vertexElements, const char* vertexSource, const char* fragmentSource);
	~Shader();

	unsigned int getId() const { return this->id; };
	unsigned int getVertexElements() const { return this->vertexElements; };

	void addAttributeArray(const char* name, unsigned int elements, unsigned int offset);
	void addAttributeArray(int location, const char* name, unsigned int elements, unsigned int offset);
	const std::vector<AttributeArray>& getAttributes() const;

	void loadUniform(std::string name, const Mat4 data);
	void loadUniform(std::string name, const Vec3 data);

	operator unsigned int() { return this->id; };
};

struct AttributeArray
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

