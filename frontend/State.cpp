#include "State.h"

State::State()
{
}

State::~State()
{
}



void State::processInput(GLFWwindow *window, double dt)
{
	if (paused)
    {
        this->previousPaused = true;
		return;
    }

    if (this->previousPaused != this->paused)
        glfwGetCursorPos(window, &this->xPos, &this->yPos);

    this->previousPaused = this->paused;

    double lastX, lastY;

	float movementSpeed = 2.5f * dt;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		movementSpeed *= 0.25;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		this->camera.moveForward(1 * movementSpeed);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		this->camera.moveBack(1 * movementSpeed);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		this->camera.moveLeft(1 * movementSpeed);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		this->camera.moveRight(1 * movementSpeed);

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		this->camera.moveDown(1 * movementSpeed);

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		this->camera.moveUp(1 * movementSpeed);

	float mouseSensitivity = 0.05f;
	lastX = this->xPos;
    lastY = this->yPos;

	glfwGetCursorPos(window, &xPos, &yPos);//coordinates are measured from the top-left corner of window
	//std::cout << (char)0xd << "dx:" << xPos-lastX << (char)0x9 << "dy:" << lastY-yPos << (char)0x9;
	this->camera.rotateHorizontal(Mat::toRads(static_cast<float>(xPos - lastX)*mouseSensitivity));
	this->camera.rotateVertical(Mat::toRads(static_cast<float>(lastY - yPos)*mouseSensitivity));
}



bool State::isPaused()
{
	return this->paused;
}

void State::togglePaused(GLFWwindow *window)
{
	this->paused = !this->paused;

	if (this->paused)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
