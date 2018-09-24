#include "Model.h"
#include <glad/glad.h>
#include <iostream>

#include "Shader.h"

namespace GLBackend
{
	Model::Model(float *vertices, unsigned int numVertices, unsigned int elementsPerVertex)
		: drawCount(numVertices), hasEBO(false)
	{
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * elementsPerVertex * numVertices, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Model::Model(float *vertices, unsigned int numVertices, unsigned int elementsPerVertex, unsigned int *EBO, unsigned int numTriangles)
		: drawCount(numTriangles), hasEBO(true)
	{
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * elementsPerVertex * numVertices, vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * numTriangles, EBO, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Model::~Model()
	{
		//TODO: this destructor could cause problems if the model is passed by value...
		if (this->hasEBO)
			glDeleteBuffers(1, &this->EBO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteVertexArrays(1, &this->VAO);

		for (auto pair : this->attributes)
		{
			delete pair.second;
		}
		this->attributes.clear();
	}

	bool Model::getHasEBO() const
	{
		return this->hasEBO;
	}

	unsigned int Model::getDrawCount() const
	{
		return this->drawCount;
	}

	void Model::addAttribute(std::string name, int size, int offset)
	{
		Model::Attribute* attr = new Model::Attribute{ size, offset };
		this->attributes.emplace(name, attr);
	}

	void Model::bindToShader(const std::shared_ptr<Shader> shader)
	{
		glBindVertexArray(this->VAO);

		if (shader->getId() != this->lastBoundShader)
		{
			this->lastBoundShader = shader->getId();

			/* // Only works on >= GL 4.3
			glBindVertexBuffer(0, this->VBO, 0, shader->getVertexElements() * sizeof(float));

			for (auto attr : shader->getAttributes())
			{
				auto iter = this->attributes.find(attr.first);
				if (iter == this->attributes.end())
					continue;

				glEnableVertexAttribArray(attr.second->location);
				glVertexAttribFormat(attr.second->location, iter->second->size, GL_FLOAT, GL_FALSE, iter->second->offset * sizeof(float));
				glVertexAttribBinding(attr.second->location, 0);
			}
			*/

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

			for (auto attr : shader->getAttributes())
			{
				auto iter = this->attributes.find(attr.first);
				if (iter == this->attributes.end())
					continue;

				glEnableVertexAttribArray(attr.second->location);
				glVertexAttribPointer(attr.second->location, iter->second->size, GL_FLOAT, GL_FALSE, shader->getVertexElements() * sizeof(float), (void*)(iter->second->offset * sizeof(float)));
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	// This is dumb.  The shader should draw the model, not model drawn with shader
	// But all the data needed to draw is in the model soooo....
	void Model::drawWithShader(const std::shared_ptr<Shader> shader)
	{
		shader->bind();

		this->bindToShader(shader);

		if (this->hasEBO)
		{
			glDrawElements(GL_TRIANGLES, this->drawCount, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, this->drawCount);
		}

		this->unbind();
		shader->unbind();
	}

	void Model::unbind() const
	{
		glBindVertexArray(0);
	}
}
