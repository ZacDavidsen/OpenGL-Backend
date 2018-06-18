#pragma once

namespace GLBackend
{
	class Model
	{
	public:
		unsigned int shaderId;
		unsigned int VAO;
		unsigned int drawCount;
		unsigned int hasEBO;

		Model();
		Model(unsigned int shaderId, unsigned int VAO, unsigned int drawCount, bool hasEBO = false);
		~Model();

		void drawModel();
	};
}

