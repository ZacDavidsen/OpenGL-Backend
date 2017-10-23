#include <glad\glad.h>
#include <GLFW\Glfw3.h>
#include <iostream>
#include "window.h"
#include "shaders.h"
#include "GLManager.h"
#include "Matrix.h"

enum Shaders
{
	SHADER_COLOR,
	SHADER_TEXTURE
};

enum Models
{
	MODEL_BOX,
	MODEL_BOX2
};

enum Textures
{
	TEXTURE_WOODEN_BOX,
	TEXTURE_TILES
};

void callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	//Vec3 vec31{1,2,3};
	//Vec3 vec32{2,3,4};
	//Vec3 vec33 = vec31 + vec32;
	//vec33.print();
	//float vals2[] = { 1,2,3,4 };
	//Mat2 mat1{1,2, 
	//		  3,4};
	//Mat2 mat2{ 1, 2, 3, 4 };
	//Mat2 mat3 = mat1 + mat2;
	//mat1.print();
	//mat3.print();

	//Mat4 mat41 = Mat4();

	//mat41.print();
	//vec31.print();

	//mat41 = Mat::translate(mat41, vec31);
	//mat41.print();

	//mat41 = Mat::scale(mat41, vec31);
	//mat41.print();

	//Mat4 mat42 = Mat4{ 
	//1,0,0,0,
	//0,1,0,0,
	//0,0,1,0,
	//0,0,0,1 };
	//mat42.print();

	//Mat4 mat43 = mat41 * mat42;
	//mat43.print();

	//MatrixHidden::Matrix<1, 4> mat1x4{ 1,1,1,1 };
	//MatrixHidden::Matrix<4, 1> mat4x1{ 1,1,1,1 };
	//(mat4x1*mat1x4).print();

	GLFWwindow* window = NULL;
	if (initWindow(600, 800, window) != 0)
	{
		return -1;
	}

	glfwSetKeyCallback(window, callback);

	//GLManager manager = GLManager();

	GLManager::createShaderProgram(SHADER_TEXTURE, 5, textureVertexSource, textureFragmentSource);
	GLManager::addShaderAttribute(SHADER_TEXTURE, "aPos", 3, 0);
	GLManager::addShaderAttribute(SHADER_TEXTURE, "aTexCoord", 2, 3);

	GLManager::createShaderProgram(SHADER_COLOR, 6, colorVertexSource, colorFragmentSource);
	GLManager::addShaderAttribute(SHADER_COLOR, "aPos", 3, 0);
	GLManager::addShaderAttribute(SHADER_COLOR, "aColor", 3, 3);

	GLManager::loadTexture(TEXTURE_WOODEN_BOX, "container.jpg");
	//manager.loadTexture(TEXTURE_TILES, "terrainTiles.png");

	/*
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.0f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	GLManager::addModel(MODEL_BOX, SHADER_TEXTURE, 4, vertices, 2, indices, TEXTURE_WOODEN_BOX);

	float vertices1[] = {
		// positions          // colors           // texture coords
		0.0f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top right
		0.0f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
		0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // bottom left
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left
	};

	manager.addModel(MODEL_BOX2, SHADER_COLOR, 4, vertices1, 2, indices);
	/*/
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLManager::addModel(MODEL_BOX, SHADER_TEXTURE, 36, vertices, TEXTURE_WOODEN_BOX);

	Vec3 cubePositions[] = {
		Vec3{0.0f,  0.0f,  0.0f},
		Vec3{2.0f,  5.0f, -15.0f},
		Vec3{-1.5f, -2.2f, -2.5f},
		Vec3{-3.8f, -2.0f, -12.3f},
		Vec3{2.4f, -0.4f, -3.5f},
		Vec3{-1.7f,  3.0f, -7.5f},
		Vec3{1.3f, -2.0f, -2.5f},
		Vec3{1.5f,  2.0f, -2.5f},
		Vec3{1.5f,  0.2f, -1.5f},
		Vec3{-1.3f,  1.0f, -1.5f}
	};
	//*/

	Mat4 trans = Mat4(1.0f);
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = Mat::scale(trans, { 0.5, 0.5, 0.5 });

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//trans = Mat::scale(trans, Vec3{ 0.5, 0.5, 0 });
		//trans = Mat::translate(trans, Vec3{ 0.5, -0.5, 0 });

		//trans2 = Mat::rotate(trans, -(float)glfwGetTime(), Vec3{ 0.0f, 0.0f, 1.0f });
		//trans = Mat::rotate(trans, (float)glfwGetTime(), Vec3{ 0.0f, 0.0f, 1.0f });

		//trans = Mat::rotate(trans, Mat::toRads(-55), Vec3{1.0f, 0, 0});
		//trans = Mat::rotate(trans, (float)glfwGetTime(), Vec3{ 0,0,1 });
		//trans = Mat::translate(trans, Vec3{ 0.0f, 0.0f, -3.0f });
		//trans = Mat::perspective(Mat::toRads(45.0f), 800 / 600, 0.1f, 100.0f)*trans;
		//trans = Mat::orthographic(8, 6, 0.1f, 100.0f)*trans;

		GLManager::loadUniform(SHADER_TEXTURE, "camera", Mat4()); 
		GLManager::loadUniform(SHADER_COLOR, "camera", Mat4());

		GLManager::loadUniform(SHADER_TEXTURE, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));
		GLManager::loadUniform(SHADER_COLOR, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));
		
		for (unsigned int i = 0; i < 10; i++)
		{
			Mat4 trans;
			trans = Mat::translate(trans, Vec3{ 0.0f, 0.0f, -3.0f });
			trans = Mat::translate(trans, cubePositions[i]);
			float angle = 20.0f * i;
			trans = Mat::rotate(trans, Mat::toRads(angle), Vec3{ 1.0f, 0.3f, 0.5f });
			GLManager::loadUniform(SHADER_TEXTURE, "model", trans);

			GLManager::drawItem(MODEL_BOX, TEXTURE_WOODEN_BOX);
		}

		//manager.drawItem(MODEL_BOX, TEXTURE_WOODEN_BOX);
		//manager.drawItem(MODEL_BOX2);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}