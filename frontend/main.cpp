#include <glad\glad.h>
#include <GLFW\Glfw3.h>

#include <iostream>
#include <sstream>
#include <string>

#include "window.h"
#include "shaders.h"
#include "GLManager.h"
#include "Matrix.h"
#include "ShaderLoader.h"

#include "Camera.h"

#include "ModelLoader.h"

enum Shaders
{
	SHADER_DEFAULT,
	SHADER_COLOR,
	SHADER_LIGHT,
	SHADER_TEXTURE
};

enum Models
{
	MODEL_BOX,
	MODEL_BOX2,
	MODEL_BOTTLE,
	MODEL_HOUSE
};

enum Textures
{
	TEXTURE_WOODEN_BOX,
	TEXTURE_TILES
};

double xPos, yPos;

void callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window, Camera& camera);
void createShaders(GLManager& man);
void loadModels(GLManager& man);

int main()
{
	GLFWwindow* window = NULL;
	if (initWindow(600, 800, window) != 0)
	{
		return -1;
	}

	glfwSetKeyCallback(window, callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//make sure that we have a starting reference point so we can get the diff each frame
	glfwGetCursorPos(window, &xPos, &yPos);

	GLManager man;
	Camera camera;

	createShaders(man);

	man.loadTexture(TEXTURE_WOODEN_BOX, "Resources/container.jpg");
	man.loadTexture(TEXTURE_TILES, "Resources/terrainTiles.png");

	loadModels(man);

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

	man.loadUniform(SHADER_TEXTURE, "camera", camera.getCameraMatrix());
	man.loadUniform(SHADER_LIGHT, "camera", camera.getCameraMatrix());
	man.loadUniform(SHADER_DEFAULT, "camera", camera.getCameraMatrix());
	//man.loadUniform(SHADER_COLOR, "camera", Mat4());

	man.loadUniform(SHADER_TEXTURE, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));
	man.loadUniform(SHADER_LIGHT, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));
	man.loadUniform(SHADER_DEFAULT, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));
	//man.loadUniform(SHADER_COLOR, "projection", Mat::perspective(Mat::toRads(45.0f), 800.0f / 600, 0.1f, 100.0f));

	Vec3 lightColor{ 1.0f, 1.0f, 1.0f };

	man.loadUniform(SHADER_LIGHT, "lightColor", lightColor);
	man.loadUniform(SHADER_TEXTURE, "lightColor", lightColor);
	man.loadUniform(SHADER_DEFAULT, "lightColor", lightColor);

	Mat4 lightTrans;
	lightTrans = Mat::scale(lightTrans, Vec3(0.2f));
	lightTrans = Mat::translate(lightTrans, Vec3{ 0.0f, 0.0f, -5.0f });
	man.loadUniform(SHADER_LIGHT, "model", lightTrans);

	man.loadUniform(SHADER_TEXTURE, "lightPos", Vec3{ 0.0f, 0.0f, -5.0f });
	man.loadUniform(SHADER_DEFAULT, "lightPos", Vec3{ 0.0f, 0.0f, -5.0f });

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

		man.loadUniform(SHADER_LIGHT, "camera", camera.getCameraMatrix());
		man.drawItem(SHADER_LIGHT, MODEL_BOX);

		man.loadUniform(SHADER_DEFAULT, "camera", camera.getCameraMatrix()); 
		man.loadUniform(SHADER_DEFAULT, "cameraPos", camera.getPosition());

		Mat4 bottleTrans;
		bottleTrans = Mat::scale(bottleTrans, Vec3(0.01f));
		bottleTrans = Mat::rotate(bottleTrans, glfwGetTime(), Vec3{ 0,1,0 });
		bottleTrans = Mat::translate(bottleTrans, Vec3{ 0.0f, 0.0f, 5.0f });
		man.loadUniform(SHADER_DEFAULT, "model", bottleTrans);
		man.drawItem(MODEL_HOUSE);

		man.loadUniform(SHADER_TEXTURE, "camera", camera.getCameraMatrix());
		man.loadUniform(SHADER_TEXTURE, "cameraPos", camera.getPosition());
		man.setTextureUniform(SHADER_TEXTURE, 2, "ourTexture", TEXTURE_WOODEN_BOX);

		for (unsigned int i = 0; i < 10; i++)
		{
			Mat4 trans;
			float angle = 20.0f * i;
			trans = Mat::rotate(trans, Mat::toRads(angle), Vec3{ 1.0f, 0.3f, 0.5f });
			trans = Mat::translate(trans, cubePositions[i]);
			man.loadUniform(SHADER_TEXTURE, "model", trans);
			man.drawItem(SHADER_TEXTURE, MODEL_BOX);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void createShaders(GLManager& man)
{
	char vertSource[1024], fragSource[1024];

	ShaderLoad::loadProgram("Resources/simpleTexture", vertSource, fragSource, 1024);

	man.createShaderProgram(SHADER_TEXTURE, 8, vertSource, fragSource);
	man.addShaderAttribute(SHADER_TEXTURE, "aPos", 3, 0);
	man.addShaderAttribute(SHADER_TEXTURE, "aTexCoord", 2, 3);
	man.addShaderAttribute(SHADER_TEXTURE, "aNorm", 3, 5);

	//man.createShaderProgram(SHADER_COLOR, 6, colorVertexSource, colorFragmentSource);
	//man.addShaderAttribute(SHADER_COLOR, "aPos", 3, 0);
	//man.addShaderAttribute(SHADER_COLOR, "aColor", 3, 3);

	man.createShaderProgram(SHADER_LIGHT, 8, lightVertexSource, lightFragmentSource);
	man.addShaderAttribute(SHADER_LIGHT, "aPos", 3, 0);

	ShaderLoad::loadProgram("Resources/default", vertSource, fragSource, 1024);

	man.createShaderProgram(SHADER_DEFAULT, 6, vertSource, fragSource);
	man.addShaderAttribute(SHADER_DEFAULT, "aPos", 3, 0);
	man.addShaderAttribute(SHADER_DEFAULT, "aNorm", 3, 3);
}

void loadModels(GLManager& man) 
{
	float vertices[] = {
		//position			  //texture	   //normal
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

	man.addModel(MODEL_BOX, SHADER_TEXTURE, 36, vertices);

	float* loadedVertices = nullptr;
	int numTriangles;

	parseObjFile("Resources/house.obj", loadedVertices, numTriangles, ModelLoaderInclude::Normals);

	//Careful here! If the shader expects more elements per vertex than the model loader supplies,
	//then the NEXT call looks like it throws an exception
	man.addModel(MODEL_HOUSE, SHADER_DEFAULT, numTriangles * 3, loadedVertices);

	delete[] loadedVertices;

	//parseObjFile("Resources/house.obj", loadedVertices, numTriangles, ModelLoaderInclude::Normals);

	//man.addModel(MODEL_BOTTLE, SHADER_DEFAULT, numTriangles * 3, loadedVertices);

	//delete[] loadedVertices;
}

void processInput(GLFWwindow *window, Camera& camera)
{
	float movementSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		movementSpeed *= 0.25;
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

void callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}
