#include "Camera.h"

//#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>

class State 
{
    double xPos = 0, yPos = 0;
    bool previousPaused = true;
    bool paused = false;

public:
    Camera camera;

    State();
    ~State();

    void processInput(GLFWwindow *window, double dt);

    bool isPaused();
    void togglePaused(GLFWwindow *window);
};