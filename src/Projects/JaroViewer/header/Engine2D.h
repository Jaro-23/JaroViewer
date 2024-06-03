#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

class Engine2D {
public:
    Engine2D(int glfwVersion, int width, int height, const char* title);

    void start();

private:
    void render();

    int mWidth;
    int mHeight;

    float mLastFrame;

    GLFWwindow *mWindow;
};
