#include "Model.h"
#include <glad\glad.h>
#include <iostream>

namespace GLBackend
{
	Model::Model()
	{
	}

	Model::Model(unsigned int shaderId, unsigned int VAO, unsigned int drawCount, bool hasEBO)
	{
		this->shaderId = shaderId;
		this->VAO = VAO;
		this->drawCount = drawCount;
		this->hasEBO = hasEBO;
	}

	Model::~Model()
	{
	}

	void Model::drawModel()
	{
		glUseProgram(shaderId);
		glBindVertexArray(VAO);

		if (this->hasEBO)
		{
			glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, drawCount);
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
}
