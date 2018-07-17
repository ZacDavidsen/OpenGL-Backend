#include <glad\glad.h>
#include <GLFW\Glfw3.h>

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

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
	SHADER_TEXTURE,
	SHADER_TEXT
};

enum Models
{
	MODEL_BOX,
	MODEL_BOTTLE,
	MODEL_HOUSE,
	MODEL_TEXT_QUAD
};

enum Textures
{
	TEXTURE_WOODEN_BOX,
	TEXTURE_TILES,
	TEXTURE_BITMAP
};

#define USE_FRAMELIMIT true
#define USE_GLFW_FRAMELIMIT true
#define FPS_UPDATE_INTERVAL 20

//TARGET_FRAMERATE only matters if using framelimit, and not using GLFW's framelimiter
#define TARGET_FRAMERATE 60

void callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window, Camera& camera, double *initx = nullptr, double *inity = nullptr);
void initShaders(GLManager& man, int screenWidth, int screenHeight);
void loadModels(GLManager& man);

int main()
{
	GLFWwindow* window = NULL;
	const int width = 800, height = 600;
	if (initWindow(width, height, window) != 0)
	{
		return -1;
	}

	glfwSetKeyCallback(window, callback);

	GLManager man;
	Camera camera;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#if !defined(USE_FRAMELIMIT) || !USE_FRAMELIMIT || !defined(USE_GLFW_FRAMELIMIT) || USE_GLFW_FRAMELIMIT == false
	glfwSwapInterval(0);
#endif
	//make sure that we have an initial value so we can get the diff each frame
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	processInput(window, camera, &xPos, &yPos);

	initShaders(man, width, height);

	man.setTexturePath("Resources/");

	man.loadTexture(TEXTURE_WOODEN_BOX, "container.jpg");
	man.loadTexture(TEXTURE_TILES, "terrainTiles.png");
	man.loadTexture(TEXTURE_BITMAP, "defaultFont.bmp");

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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	double startTime, currentTime = glfwGetTime(), dt, lastFpsTime = currentTime;
	char frameCounter = 0;
	std::string fps;
	
	while (!glfwWindowShouldClose(window))
	{
		startTime = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window, camera);

		man.loadUniform(SHADER_LIGHT, "camera", camera.getCameraMatrix());
		man.drawItem(SHADER_LIGHT, MODEL_BOX);

		man.loadUniform(SHADER_DEFAULT, "camera", camera.getCameraMatrix()); 
		man.loadUniform(SHADER_DEFAULT, "cameraPos", camera.getPosition());

		Mat4 bottleTrans;
		bottleTrans = Mat::scale(bottleTrans, Vec3(0.01f));
		bottleTrans = Mat::rotate(bottleTrans, static_cast<float>(glfwGetTime()), Vec3{ 0,1,0 });
		bottleTrans = Mat::translate(bottleTrans, Vec3{ 0.0f, 0.0f, 7.0f });
		man.loadUniform(SHADER_DEFAULT, "model", bottleTrans);
		man.drawItem(SHADER_DEFAULT, MODEL_HOUSE);

		man.loadUniform(SHADER_TEXTURE, "camera", camera.getCameraMatrix());
		man.loadUniform(SHADER_TEXTURE, "cameraPos", camera.getPosition());
		man.setTextureUniform(SHADER_TEXTURE, 0, "ourTexture", TEXTURE_WOODEN_BOX);

		for (unsigned int i = 0; i < 10; i++)
		{
			Mat4 trans;
			float angle = 20.0f * i;
			trans = Mat::rotate(trans, Mat::toRads(angle), Vec3{ 1.0f, 0.3f, 0.5f });
			trans = Mat::translate(trans, cubePositions[i]);
			man.loadUniform(SHADER_TEXTURE, "model", trans);
			man.drawItem(SHADER_TEXTURE, MODEL_BOX);
		}

		glDisable(GL_DEPTH_TEST);

		man.setTextureUniform(SHADER_TEXT, 0, "text", TEXTURE_BITMAP);

		Mat4 textTrans;
		textTrans = Mat::scale(textTrans, Vec3{ 20, 40, 0 });
		textTrans = Mat::translate(textTrans, Vec3{ 5, height - 30, 0 });

		for (int i = 0; i < fps.length(); i++)
		{
			man.loadUniform(SHADER_TEXT, "model", textTrans);
			man.loadUniform(SHADER_TEXT, "character", fps[i]);
			man.drawItem(SHADER_TEXT, MODEL_TEXT_QUAD);

			textTrans = Mat::translate(textTrans, Vec3{ 15, 0, 0 });
		}

		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();

		currentTime = glfwGetTime();

		if ((frameCounter %= FPS_UPDATE_INTERVAL)++ == 0)
		{
			fps = std::to_string(static_cast<int>(1.0f * FPS_UPDATE_INTERVAL / (currentTime - lastFpsTime)));
			lastFpsTime = currentTime;
		}

#if defined(USE_FRAMELIMIT) && USE_FRAMELIMIT && defined(USE_GLFW_FRAMELIMIT) && USE_GLFW_FRAMELIMIT == false
		dt = glfwGetTime() - startTime;

		if (dt < (1.0f / TARGET_FRAMERATE))
		{
			std::this_thread::sleep_for(std::chrono::microseconds(static_cast<long>((1.0f / TARGET_FRAMERATE - dt) * 1000000)));
		}
#endif
	}

	glfwTerminate();
	return 0;
}

void initShaders(GLManager& man, int screenWidth, int screenHeight)
{
	Mat4 perspective = Mat::perspective(Mat::toRads(45.0f), static_cast<float>(screenWidth) / screenHeight, 0.1f, 100.0f);
	Mat4 textOrtho = Mat::orthographic(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight), 0.0f, 100.0f);
	Vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	Vec3 lightPosition{ 0.0f, 0.0f, -5.0f };

	char vertSource[1024], fragSource[1024];

	ShaderLoad::loadProgram("Resources/simpleTexture", vertSource, fragSource, 1024);
	man.createShaderProgram(SHADER_TEXTURE, 8, vertSource, fragSource);
	man.addShaderAttribute(SHADER_TEXTURE, "aPos", 3, 0);
	man.addShaderAttribute(SHADER_TEXTURE, "aTexCoord", 2, 3);
	man.addShaderAttribute(SHADER_TEXTURE, "aNorm", 3, 5);
	man.loadUniform(SHADER_TEXTURE, "projection", perspective);
	man.loadUniform(SHADER_TEXTURE, "lightColor", lightColor);
	man.loadUniform(SHADER_TEXTURE, "lightPos", lightPosition);

	//man.createShaderProgram(SHADER_COLOR, 6, colorVertexSource, colorFragmentSource);
	//man.addShaderAttribute(SHADER_COLOR, "aPos", 3, 0);
	//man.addShaderAttribute(SHADER_COLOR, "aColor", 3, 3);
	//man.loadUniform(SHADER_COLOR, "projection", perspective);

	man.createShaderProgram(SHADER_LIGHT, 8, lightVertexSource, lightFragmentSource);
	man.addShaderAttribute(SHADER_LIGHT, "aPos", 3, 0);
	man.loadUniform(SHADER_LIGHT, "projection", perspective);
	man.loadUniform(SHADER_LIGHT, "lightColor", lightColor);
	Mat4 lightTrans;
	lightTrans = Mat::scale(lightTrans, Vec3(0.2f));
	lightTrans = Mat::translate(lightTrans, lightPosition);
	man.loadUniform(SHADER_LIGHT, "model", lightTrans);

	ShaderLoad::loadProgram("Resources/default", vertSource, fragSource, 1024);
	man.createShaderProgram(SHADER_DEFAULT, 6, vertSource, fragSource);
	man.addShaderAttribute(SHADER_DEFAULT, "aPos", 3, 0);
	man.addShaderAttribute(SHADER_DEFAULT, "aNorm", 3, 3);
	man.loadUniform(SHADER_DEFAULT, "projection", perspective);
	man.loadUniform(SHADER_DEFAULT, "lightColor", lightColor);
	man.loadUniform(SHADER_DEFAULT, "lightPos", lightPosition);

	ShaderLoad::loadProgram("Resources/simpleText", vertSource, fragSource, 1024);
	man.createShaderProgram(SHADER_TEXT, 2, vertSource, fragSource);
	man.addShaderAttribute(SHADER_TEXT, "aPos", 2, 0);
	man.loadUniform(SHADER_TEXT, "projection", textOrtho);
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

	man.addModel(MODEL_BOX, vertices, 36, 8);

	float* loadedVertices = nullptr;
	int numTriangles;

	parseObjFile("Resources/house.obj", loadedVertices, numTriangles, ModelLoaderInclude::Normals);

	//Careful here! If the shader expects more elements per vertex than the model loader supplies,
	//then the NEXT call looks like it throws an exception
	man.addModel(MODEL_HOUSE, loadedVertices, numTriangles * 3, 6);

	delete[] loadedVertices;

	//parseObjFile("Resources/house.obj", loadedVertices, numTriangles, ModelLoaderInclude::Normals);

	//man.addModel(MODEL_BOTTLE, SHADER_DEFAULT, numTriangles * 3, loadedVertices);

	//delete[] loadedVertices;

	float textVerts[] = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	man.addModel(MODEL_TEXT_QUAD, textVerts, 6, 2);
}

void processInput(GLFWwindow *window, Camera &camera, double *initx, double *inity)
{
	//don't really like this, but it removes the global variables..
	static double xPos, yPos;
	if (initx != nullptr && inity != nullptr)
	{
		xPos = *initx;
		yPos = *inity;
		return;
	}

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
	double lastX = xPos, lastY = yPos;

	glfwGetCursorPos(window, &xPos, &yPos);//coordinates are measured from the top-left corner of window
										   //std::cout << (char)0xd << "dx:" << xPos-lastX << (char)0x9 << "dy:" << lastY-yPos << (char)0x9;
	camera.rotateHorizontal(Mat::toRads(static_cast<float>(xPos - lastX)*mouseSensitivity));
	camera.rotateVertical(Mat::toRads(static_cast<float>(lastY - yPos)*mouseSensitivity));
}

void callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}
