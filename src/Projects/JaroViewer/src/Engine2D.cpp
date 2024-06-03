#include "../header/Engine2D.h"

#include <iostream>

Engine2D::Engine2D(int glfwVersion, int width, int height, const char* title) :
	mWidth{width}, mHeight{height}, mLastFrame{0.0f}
{
	// Start up GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the needed window
	mWindow = glfwCreateWindow(mWidth, mHeight, title, NULL, NULL);
	if (mWindow == NULL) {
		glfwTerminate();
		std::cerr << "[Engine 2D] Failed to create a GLFW window" << std::endl;
		return;
	}
	glfwMakeContextCurrent(mWindow);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "[Engine 2D] Failed to initialize GLAD" << std::endl;
		return;
	}

	glViewport(0, 0, mWidth, mHeight);
}

/**
 * Starts the engine and opens the window
 */
void Engine2D::start() {
	// TODO: do the setup

	mLastFrame = 0.0f;
	render();
	glfwTerminate();
}

/**
 * Contains the main rendering loop 
 */
void Engine2D::render() {
	float deltaTime = 0.0f;
	while (!glfwWindowShouldClose(mWindow)) {
		deltaTime = glfwGetTime() - mLastFrame;
		mLastFrame = deltaTime;

		// TODO: do all the rendering

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}