#pragma once

class Model
{
	unsigned int shaderId;
	unsigned int VAO;
	unsigned int drawCount;
	unsigned int hasEBO;
public:
	Model();
	Model(unsigned int shaderId, unsigned int VAO, unsigned int drawCount, bool hasEBO = false);
	~Model();

	void drawModel(unsigned int textureId = 0);
};

