#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

int initWindow(int width, int height, GLFWwindow*& windowOut, bool useDebugContext = false);