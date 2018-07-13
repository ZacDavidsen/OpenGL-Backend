#include "Model.h"
#include <glad\glad.h>
#include <iostream>

#include "Shader.h"

namespace GLBackend
{
	Model::Model(float *vertices, unsigned int numVertices, unsigned int numVertexElements) 
		: drawCount(numVertices), hasEBO(false)
	{
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertexElements * numVertices, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Model::Model(float *vertices, unsigned int numVertices, unsigned int numVertexElements, unsigned int *EBO, unsigned int numTriangles) 
		: drawCount(numTriangles), hasEBO(true)
	{
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertexElements * numVertices, vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * numTriangles, EBO, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Model::~Model()
	{
		if (this->hasEBO)
			glDeleteBuffers(1, &this->EBO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteVertexArrays(1, &this->VAO);
	}

	bool Model::getHasEBO() const
	{
		return this->hasEBO;
	}

	unsigned int Model::getDrawCount() const
	{
		return this->drawCount;
	}

	void Model::bindToShader(Shader const *shader)
	{
		glBindVertexArray(this->VAO);

		if (shader->getId() != this->lastBoundShader)
		{
			this->lastBoundShader = shader->getId();

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

			for (auto attr : shader->getAttributes())
			{
				glVertexAttribPointer(attr->location, attr->size, GL_FLOAT, GL_FALSE, shader->getVertexElements() * sizeof(float), (void*)(attr->offset * sizeof(float)));
				glEnableVertexAttribArray(attr->location);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	void Model::unbind() const
	{
		glBindVertexArray(0);
	}
}
