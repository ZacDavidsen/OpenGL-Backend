#include <glad\glad.h>
#include <GLFW\Glfw3.h>
#include <iostream>
#include "window.h"
#include "shaders.h"
#include "GLManager.h"
#include "Matrix.h"
#include "ShaderLoader.h"

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

void processInput(GLFWwindow *window, Camera& camera)
{
	float movementSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(1 * movementSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveBack(1 * movementSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveLeft(1 * movementSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(1 * movementSpeed);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.moveDown(1 * movementSpeed);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.moveUp(1 * movementSpeed);

	float mouseSensitivity = 0.05f;
	float lastX = (float)xPos, lastY = (float)yPos;

	glfwGetCursorPos(window, &xPos, &yPos);//coordinates are measured from the top-left corner of window
	//std::cout << (char)0xd << "dx:" << xPos-lastX << (char)0x9 << "dy:" << lastY-yPos << (char)0x9;
	camera.rotateHorizontal(Mat::toRads((float)(xPos - lastX)*mouseSensitivity));
	camera.rotateVertical(Mat::toRads((float)(lastY - yPos)*mouseSensitivity));
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

	GLManager* man = new GLManager();
	Camera camera = Camera();

	char vertSource[1024], fragSource[1024];

	ShaderLoad::loadProgram("simpleTexture", vertSource, fragSource, 1024);
	
	man->createShaderProgram(SHADER_TEXTURE, 8, vertSource, fragSource);
	man->addShaderAttribute(SHADER_TEXTURE, "aPos", 3, 0);
	man->addShaderAttribute(SHADER_TEXTURE, "aTexCoord", 2, 3);
	man->addShaderAttribute(SHADER_TEXTURE, "aNorm", 3, 5);

	//man->createShaderProgram(SHADER_COLOR, 6, colorVertexSource, colorFragmentSource);
	//man->addShaderAttribute(SHADER_COLOR, "aPos", 3, 0);
	//man->addShaderAttribute(SHADER_COLOR, "aColor", 3, 3);

	man->createShaderProgram(SHADER_LIGHT, 8, lightVertexSource, lightFragmentSource);
	man->addShaderAttribute(SHADER_LIGHT, "aPos", 3, 0);

	man->loadTexture(TEXTURE_WOODEN_BOX, "container.jpg");
	man->loadTexture(TEXTURE_TILES, "terrainTiles.png");

	float vertices[] = {
	//position			  //texture	   //color
	//back side
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
	//front side
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	//left side
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
	//right side
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	//bottom side
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	//top side
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
	};
#define DRAW_ONE_SIDEN
#ifdef DRAW_ONE_SIDE
	man->addModel(MODEL_BOX, SHADER_TEXTURE, 6, vertices + 2 * 6 * 8);
#else
	man->addModel(MODEL_BOX, SHADER_TEXTURE, 36, vertices);
#endif

	man->addModel(MODEL_BOX2, SHADER_LIGHT, 36, vertices);

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

	camera.setPosition({ 0.0f, 0.0f, 3.0f });

	man->loadUniform(SHADER_TEXTURE, "camera", camera.getCameraMatrix());
	man->loadUniform(SHADER_LIGHT, "camera", camera.getCameraMatrix());
	//man->loadUniform(SHADER_COLOR, "camera", Mat4());

	man->loadUniform(SHADER_TEXTURE, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));
	man->loadUniform(SHADER_LIGHT, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));
	//man->loadUniform(SHADER_COLOR, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));

	Vec3 lightColor{ 10.0f, 0.0f, 1.0f };

	man->loadUniform(SHADER_LIGHT, "lightColor", lightColor);
	man->loadUniform(SHADER_TEXTURE, "lightColor", lightColor);
	Mat4 lightTrans;
	lightTrans = Mat::scale(lightTrans, Vec3(0.2f));
	lightTrans = Mat::translate(lightTrans, Vec3{ 0.0f, 0.0f, -5.0f });
	man->loadUniform(SHADER_LIGHT, "model", lightTrans);
	man->loadUniform(SHADER_TEXTURE, "lightPos", Vec3{ 0.0f, 0.0f, -5.0f });

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//float camX = (float)sin(glfwGetTime()) * 10;
		//float camZ = (float)cos(glfwGetTime()) * 10;

		processInput(window, camera);
		//Camera::setPosition({ camX, 0, camZ });
		//Camera::setTarget({ 0,0,0 });
		man->loadUniform(SHADER_TEXTURE, "camera", camera.getCameraMatrix());
		man->loadUniform(SHADER_TEXTURE, "cameraPos", camera.getPosition());
		man->setTextureUniform(SHADER_TEXTURE, 2, "ourTexture", TEXTURE_WOODEN_BOX);

		man->loadUniform(SHADER_LIGHT, "camera", camera.getCameraMatrix());

		man->drawItem(SHADER_LIGHT, MODEL_BOX);

		for (unsigned int i = 0; i < 10; i++)
		{
			Mat4 trans;
			//trans = Mat::translate(trans, Vec3{ 0.0f, 0.0f, -3.0f });
			trans = Mat::translate(trans, cubePositions[i]);
			float angle = 20.0f * i;
			trans = Mat::rotate(trans, Mat::toRads(angle), Vec3{ 1.0f, 0.3f, 0.5f });
			man->loadUniform(SHADER_TEXTURE, "model", trans);
			man->drawItem(SHADER_TEXTURE, MODEL_BOX);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete man;

	glfwTerminate();
	return 0;
}