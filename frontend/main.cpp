#include <glad\glad.h>
#include <GLFW\Glfw3.h>
#include <iostream>
#include "window.h"
#include "shaders.h"
#include "GLManager.h"
#include "Matrix.h"

#include "Camera.h"

enum Shaders
{
	SHADER_COLOR,
	SHADER_LIGHT,
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

double xPos, yPos;

void processInput(GLFWwindow *window)
{
	float movementSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Camera::moveForward(1 * movementSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Camera::moveBack(1 * movementSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Camera::moveLeft(1 * movementSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Camera::moveRight(1 * movementSpeed);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		Camera::moveDown(1 * movementSpeed);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		Camera::moveUp(1 * movementSpeed);

	float mouseSensitivity = 0.05f;
	float lastX = xPos, lastY = yPos;

	glfwGetCursorPos(window, &xPos, &yPos);//coordinates are measured from the top-left corner of window
	//std::cout << (char)0xd << "dx:" << xPos-lastX << (char)0x9 << "dy:" << lastY-yPos << (char)0x9;
	Camera::rotateHorizontal(Mat::toRads((xPos - lastX)*mouseSensitivity));
	Camera::rotateVertical(Mat::toRads((lastY - yPos)*mouseSensitivity));
}

int main()
{

	GLFWwindow* window = NULL;
	if (initWindow(600, 800, window) != 0)
	{
		return -1;
	}

	glfwSetKeyCallback(window, callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(window, &xPos, &yPos);

	//GLManager manager = GLManager();

	GLManager::createShaderProgram(SHADER_TEXTURE, 8, textureVertexSource, textureFragmentSource);
	GLManager::addShaderAttribute(SHADER_TEXTURE, "aPos", 3, 0);
	GLManager::addShaderAttribute(SHADER_TEXTURE, "aTexCoord", 2, 3);
	GLManager::addShaderAttribute(SHADER_TEXTURE, "aNorm", 3, 5);

	//GLManager::createShaderProgram(SHADER_COLOR, 6, colorVertexSource, colorFragmentSource);
	//GLManager::addShaderAttribute(SHADER_COLOR, "aPos", 3, 0);
	//GLManager::addShaderAttribute(SHADER_COLOR, "aColor", 3, 3);

	GLManager::createShaderProgram(SHADER_LIGHT, 8, lightVertexSource, lightFragmentSource);
	GLManager::addShaderAttribute(SHADER_LIGHT, "aPos", 3, 0);

	GLManager::loadTexture(TEXTURE_WOODEN_BOX, "container.jpg");
	GLManager::loadTexture(TEXTURE_TILES, "terrainTiles.png");

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
	};

	GLManager::addModel(MODEL_BOX, SHADER_TEXTURE, 36, vertices);
	GLManager::addModel(MODEL_BOX2, SHADER_LIGHT, 36, vertices);

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

	Camera::setPosition({ 0.0f, 0.0f, 2.0f });

	GLManager::loadUniform(SHADER_TEXTURE, "camera", Camera::getCameraMatrix());
	GLManager::loadUniform(SHADER_LIGHT, "camera", Camera::getCameraMatrix());
	//GLManager::loadUniform(SHADER_COLOR, "camera", Mat4());

	GLManager::loadUniform(SHADER_TEXTURE, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));
	GLManager::loadUniform(SHADER_LIGHT, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));
	//GLManager::loadUniform(SHADER_COLOR, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));

	Vec3 lightColor{ 1.0f, 0.5f, 0.5f };

	GLManager::loadUniform(SHADER_LIGHT, "lightColor", lightColor);
	GLManager::loadUniform(SHADER_TEXTURE, "lightColor", lightColor);
	Mat4 lightTrans;
	lightTrans = Mat::scale(lightTrans, Vec3(0.2f));
	lightTrans = Mat::translate(lightTrans, Vec3{ 0.0f, 0.0f, -5.0f });
	GLManager::loadUniform(SHADER_LIGHT, "model", lightTrans);
	GLManager::loadUniform(SHADER_TEXTURE, "lightPos", Vec3{ 0.0f, 0.0f, -5.0f });

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float camX = sin(glfwGetTime()) * 10;
		float camZ = cos(glfwGetTime()) * 10;

		processInput(window);
		//Camera::setPosition({ camX, 0, camZ });
		//Camera::setTarget({ 0,0,0 });
		GLManager::loadUniform(SHADER_TEXTURE, "camera", Camera::getCameraMatrix());
		GLManager::loadUniform(SHADER_LIGHT, "camera", Camera::getCameraMatrix());

		GLManager::drawItem(MODEL_BOX2);

		for (unsigned int i = 0; i < 10; i++)
		{
			Mat4 trans;
			//trans = Mat::translate(trans, Vec3{ 0.0f, 0.0f, -3.0f });
			trans = Mat::translate(trans, cubePositions[i]);
			float angle = 20.0f * i;
			trans = Mat::rotate(trans, Mat::toRads(angle), Vec3{ 1.0f, 0.3f, 0.5f });
			GLManager::loadUniform(SHADER_TEXTURE, "model", trans);
			GLManager::setTextureUniform(SHADER_TEXTURE, 2, "ourTexture", TEXTURE_WOODEN_BOX);
			GLManager::drawItem(MODEL_BOX);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}