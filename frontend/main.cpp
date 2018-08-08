#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>

#include "window.h"
#include "State.h"
#include "Entity.h"

#include "GLManager.h"
#include "Matrix.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "ModelLoader.h"
#include "Shader.h"
#include "Model.h"

namespace Resources
{
	enum class Shader : int
	{
		DEFAULT,
		COLOR,
		LIGHT,
		TEXTURE,
		TEXT
	};

	enum class Model: int
	{
		BOX,
		BOTTLE,
		HOUSE,
		TEXT_QUAD
	};

	enum class Texture : int
	{
		WOODEN_BOX,
		TILES,
		BITMAP
	};
}


#define USE_FRAMELIMIT true
#define USE_GLFW_FRAMELIMIT true
#define FPS_UPDATE_INTERVAL 20

//TARGET_FRAMERATE only matters if using framelimit, and not using GLFW's framelimiter
#define TARGET_FRAMERATE 60

#define ENABLE_DEBUGGING_OUTPUT false

void drawText(float screenXPos, float screenYPos, std::string text, std::shared_ptr<GLBackend::Shader> shader, std::shared_ptr<GLBackend::Model> model, int texture);
void initShaders(GLManager& man, int screenWidth, int screenHeight);
void loadModels(GLManager& man);
void callback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main()
{
	GLFWwindow* window = nullptr;
	const int width = 800, height = 600;

#if defined(ENABLE_DEBUGGING_OUTPUT) && ENABLE_DEBUGGING_OUTPUT == true
	bool debugging = true;
#else
	bool debugging = false;
#endif

	if (initWindow(width, height, window, debugging) != 0)
	{
		return -1;
	}

	glfwSetKeyCallback(window, callback);

	GLManager man;
	State state;

	glfwSetWindowUserPointer(window, &state);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#if defined(USE_FRAMELIMIT) && USE_FRAMELIMIT && defined(USE_GLFW_FRAMELIMIT) && USE_GLFW_FRAMELIMIT == true
	glfwSwapInterval(1);
#else
	glfwSwapInterval(0);
#endif

	if (debugging)
		man.enableDebugOutput();

	initShaders(man, width, height);

	std::shared_ptr<GLBackend::Shader> lightShad, defaultShad, textureShad, textShad;

	lightShad = man.getShader(Resources::Shader::LIGHT);
	defaultShad = man.getShader(Resources::Shader::DEFAULT);
	textureShad = man.getShader(Resources::Shader::TEXTURE);
	textShad = man.getShader(Resources::Shader::TEXT);

	man.setTexturePath("Resources/");

	man.loadTexture(Resources::Texture::WOODEN_BOX, "container.jpg");
	man.loadTexture(Resources::Texture::TILES, "terrainTiles.png");
	man.loadTexture(Resources::Texture::BITMAP, "defaultFont.bmp");

	loadModels(man);

	std::shared_ptr<GLBackend::Model> boxMod, houseMod, textMod;
	boxMod = man.getModel(Resources::Model::BOX);
	houseMod = man.getModel(Resources::Model::HOUSE);
	textMod = man.getModel(Resources::Model::TEXT_QUAD);

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

	std::vector<Entity> cubes;

	for (int i = 0; i < 10; i++)
	{
		cubes.push_back(Entity(boxMod, textureShad));
		cubes[i].setPosition(cubePositions[i]);
		cubes[i].setRotation(Mat::toRads(20.0f) * i, Vec3{ 1.0f, 0.3f, 0.5f });
	}

	Entity house(houseMod, defaultShad);
	house.scaleBy(Vec3(0.01f));
	house.translateBy(Vec3{ 0.0f, 0.0f, 7.0f });

	Entity light(boxMod, lightShad);
	light.scaleBy(Vec3(0.2f));
	light.setPosition(Vec3{ 0.0f, 0.0f, -5.0f });

	state.camera.setPosition(Vec3{ 0.0f, 0.0f, 3.0f });

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	double startTime, currentTime, dt, lastFpsTime;
	startTime = lastFpsTime = glfwGetTime();
	char frameCounter = 1;
	std::string fps;
	
	while (!glfwWindowShouldClose(window))
	{
		dt = state.isPaused() ? 0 : glfwGetTime() - startTime;
		startTime = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		state.processInput(window, dt);

		lightShad->setUniform("camera", state.camera.getCameraMatrix());
		light.draw();

		defaultShad->setUniform("camera", state.camera.getCameraMatrix()); 
		defaultShad->setUniform("cameraPos", state.camera.getPosition());

		textureShad->setUniform("camera", state.camera.getCameraMatrix());
		textureShad->setUniform("cameraPos", state.camera.getPosition());
		textureShad->setTexture(0, "ourTexture", man.getTexture(Resources::Texture::WOODEN_BOX));

		house.rotateBy(Vec3{ 0.0f, 0.0f, static_cast<float>(dt) });
		house.draw();

		for (unsigned int i = 0; i < 10; i++)
		{
			cubes[i].draw();
		}

		drawText(5, height - 30, fps, textShad, textMod, man.getTexture(Resources::Texture::BITMAP));
		if (state.isPaused()) 
			drawText(5, height - 50, "Paused", textShad, textMod, man.getTexture(Resources::Texture::BITMAP));

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

void drawText(float screenXPos, float screenYPos, std::string text, std::shared_ptr<GLBackend::Shader> shader, std::shared_ptr<GLBackend::Model> model, int texture)
{
	shader->setTexture(0, "text", texture);

	glDisable(GL_DEPTH_TEST);

	Mat4 textTrans;
	textTrans = Mat::scale(textTrans, Vec3{ 20, 40, 0 });
	textTrans = Mat::translate(textTrans, Vec3{ screenXPos, screenYPos, 0 });

	for (int i = 0; i < text.length(); i++)
	{
		shader->setUniform("model", textTrans);
		shader->setUniform("character", text[i]);
		model->drawWithShader(shader);
		//textShad->draw(textMod);

		textTrans = Mat::translate(textTrans, Vec3{ 15, 0, 0 });
	}

	glEnable(GL_DEPTH_TEST);
}

void initShaders(GLManager& man, int screenWidth, int screenHeight)
{
	Mat4 perspective = Mat::perspectiveFOV(Mat::toRads(45.0f), static_cast<float>(screenWidth) / screenHeight, 0.1f, 100.0f);
	Mat4 textOrtho = Mat::orthographic(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight), 0.0f, 100.0f);
	Vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	Vec3 lightPosition{ 0.0f, 0.0f, -5.0f };

	char vertSource[1024], fragSource[1024];
	std::shared_ptr<GLBackend::Shader> temp;

	ShaderLoad::loadProgram("Resources/simpleTexture", vertSource, fragSource, 1024);
	temp = man.createShaderProgram(Resources::Shader::TEXTURE, 8, vertSource, fragSource);
	temp->addAttribute("aPos", 3, 0);
	temp->addAttribute("aTexCoord", 2, 3);
	temp->addAttribute("aNorm", 3, 5);
	temp->setUniform("projection", perspective);
	temp->setUniform("lightColor", lightColor);
	temp->setUniform("lightPos", lightPosition);

	//ShaderLoad::loadProgram("Resources/color", vertSource, fragSource, 1024);
	//man.createShaderProgram(Shader::COLOR, 6, vertSource, fragSource);
	//man.addShaderAttribute(Shader::COLOR, "aPos", 3, 0);
	//man.addShaderAttribute(Shader::COLOR, "aColor", 3, 3);
	//man.loadUniform(Shader::COLOR, "projection", perspective);

	ShaderLoad::loadProgram("Resources/light", vertSource, fragSource, 1024);
	temp = man.createShaderProgram(Resources::Shader::LIGHT, 8, vertSource, fragSource);
	temp->addAttribute("aPos", 3, 0);
	temp->setUniform("projection", perspective);
	temp->setUniform("lightColor", lightColor);

	ShaderLoad::loadProgram("Resources/default", vertSource, fragSource, 1024);
	temp = man.createShaderProgram(Resources::Shader::DEFAULT, 6, vertSource, fragSource);
	temp->addAttribute("aPos", 3, 0);
	temp->addAttribute("aNorm", 3, 3);
	temp->setUniform("projection", perspective);
	temp->setUniform("lightColor", lightColor);
	temp->setUniform("lightPos", lightPosition);

	ShaderLoad::loadProgram("Resources/simpleText", vertSource, fragSource, 1024);
	temp = man.createShaderProgram(Resources::Shader::TEXT, 2, vertSource, fragSource);
	temp->addAttribute("aPos", 2, 0);
	temp->setUniform("projection", textOrtho);
}

void loadModels(GLManager& man) 
{
	float vertices[] = {
		//position			  //textureShad	   //normal
		//back side
		 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
		//front side
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f,
		//left side
		-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
		//right side
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
		//bottom side
		-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
		//top side
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  1.0f,  0.0f, 
		-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f
	};

	std::shared_ptr<GLBackend::Model> temp;

	temp = man.addModel(Resources::Model::BOX, vertices, 36, 8);
	temp->addAttribute("aPos", 3, 0);
	temp->addAttribute("aTexCoord", 2, 3);
	temp->addAttribute("aNorm", 3, 5);

	float* loadedVertices = nullptr;
	int numTriangles;

	parseObjFile("Resources/house.obj", loadedVertices, numTriangles, ModelLoaderInclude::Normals);

	temp = man.addModel(Resources::Model::HOUSE, loadedVertices, numTriangles * 3, 6);
	temp->addAttribute("aPos", 3, 0);
	temp->addAttribute("aNorm", 3, 3);

	delete[] loadedVertices;

	//parseObjFile("Resources/house.obj", loadedVertices, numTriangles, ModelLoaderInclude::Normals);

	//man.addModel(MODEL_BOTTLE, Shader::DEFAULT, numTriangles * 3, loadedVertices);

	//delete[] loadedVertices;

	float textVerts[] = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	temp = man.addModel(Resources::Model::TEXT_QUAD, textVerts, 6, 2);
	temp->addAttribute("aPos", 2, 0);
}

void callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		State *state = (State*)glfwGetWindowUserPointer(window);

		state->togglePaused(window);
	}
}
