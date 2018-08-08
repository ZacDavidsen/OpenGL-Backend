#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

#include "MatrixTypes.h"

namespace GLBackend
{
	struct Attribute;
	struct Uniform;
	
	class Model;

	class Shader : public std::enable_shared_from_this<Shader>
	{
	public:
		struct Attribute;
		struct Uniform;

	private:
		bool creationSucceeded;
		char *infoLog = nullptr;

		unsigned int programID;
		//TODO break shader stages into their own class, for reuse, better management
		unsigned int vertexShaderID;
		unsigned int fragmentShaderID;
		unsigned int vertexElements;

		std::unordered_map<std::string, Attribute*> attributes;
		std::unordered_map<std::string, Uniform*> uniforms;

		int getUniformLocation(std::string name);

	public:
		Shader(unsigned int vertexElements, const char* vertexSource, const char* fragmentSource);
		~Shader();

		bool isGood() const { return this->creationSucceeded; };
		char const* getInfoLog() const { return this->infoLog; };

		void bind() const;
		void unbind() const;

		unsigned int getId() const { return this->programID; };
		unsigned int getVertexElements() const { return this->vertexElements; };

		void addAttribute(const char* name, unsigned int elements, unsigned int offset);
		void addAttribute(int location, const char* name, unsigned int elements, unsigned int offset);
		const std::unordered_map<std::string, Shader::Attribute*>& getAttributes() const;

		void setUniform(std::string name, const Mat2 &data);
		void setUniform(std::string name, const Mat3 &data);
		void setUniform(std::string name, const Mat4 &data);

		void setUniform(std::string name, const Vec2 &data);
		void setUniform(std::string name, const iVec2 &data);
		void setUniform(std::string name, const uVec2 &data);
		//void setUniform(std::string name, const bVec2 data);

		void setUniform(std::string name, const Vec3 &data);
		void setUniform(std::string name, const iVec3 &data);
		void setUniform(std::string name, const uVec3 &data);
		//void setUniform(std::string name, const bVec3 data);

		void setUniform(std::string name, const Vec4 &data);
		void setUniform(std::string name, const iVec4 &data);
		void setUniform(std::string name, const uVec4 &data);
		//void setUniform(std::string name, const bVec4 data);

		void setUniform(std::string name, float data);
		void setUniform(std::string name, int data);
		void setUniform(std::string name, unsigned int data);
		void setUniform(std::string name, bool data);

		void setTexture(int textureSlot, std::string uniformName, int textureId);


		// void draw(std::shared_ptr<Model> model);


		struct Attribute
		{
			int location;
			unsigned int size;
			//dataType
			//unsigned int offset;
		};

		struct Uniform
		{
			int location;
			//unsigned int dataType;
		};
	};


}
