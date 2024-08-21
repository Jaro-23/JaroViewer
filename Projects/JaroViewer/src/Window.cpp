#include "../header/Window.h"
#include "GLFW/glfw3.h"

#include <GLM/gtc/matrix_transform.hpp>
#include <iostream>

using namespace JaroViewer;

/**
 * Creates a window
 * @param glfwVersion The version of glfw that will be used
 * @param width The width of the screen
 * @param height The height of the screen
 * @param title The title of the window
 */
Window::Window(int glfwVersion, int width, int height, const std::string &title) :
	mWidth{width},
	mHeight{height}
{
	// Start up GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the needed window
	mWindow = glfwCreateWindow(mWidth, mHeight, title.c_str(), NULL, NULL);
	if (mWindow == NULL) {
		glfwTerminate();
		std::cerr << "ERROR::WINDOW::FAILED_TO_CREATE_WINDOW" << std::endl;
		return;
	}
	glfwMakeContextCurrent(mWindow);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "ERROR::WINDOW::FAILED_TO_INITIALIZE_GLAD" << std::endl;
		return;
	}

	// Setting up the viewport
	glViewport(0, 0, mWidth, mHeight);
	glEnable(GL_DEPTH_TEST);
}

/**
 * Clears the window
 * @post the window will be fully black
 */
void Window::clearWindow() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * Updates the screen
 */
void Window::update() {
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

/**
 * Updates the viewport and size so the viewport will fill the entire screen
 * @return Will return true if the window has been changed in size
 */
bool Window::updateView() {
	int width, height;
	glfwGetWindowSize(mWindow, &width, &height);
	if (mWidth == width && mHeight == height) return false;

	mWidth = width;
	mHeight = height;
	glViewport(0, 0, width, height);
	return true;
}

/**
 * Sets the should close variable
 * @param close The new value of the should close variable
 */
void Window::setShouldClose(bool close) {
	glfwSetWindowShouldClose(mWindow, close);
}

/**
 * Returns the should close variable
 * @return The value of the should close variable
 */
bool Window::shouldClose() const { return glfwWindowShouldClose(mWindow); }

/**
 * Returns the pointer to the GLFWwindow pointer
 * @return A pointer to the GLFWwindow
 */
GLFWwindow* Window::getPointer() const { return mWindow; }

/**
 * Returns the projection matrix
 * @return A projection matrix
 */
glm::mat4 Window::getProjection() const {
	return glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
}

/**
 * Returns if a key is pressed on the window
 * @return True if the key is pressed otherwise false
 */
bool Window::isKeyPressed(int key) const {
	return glfwGetKey(mWindow, key) == GLFW_PRESS;
}

/**
 * Returns if a key is released on the window
 * @return True if the key is pressed otherwise false
 */
bool Window::isKeyReleased(int key) const {
	return glfwGetKey(mWindow, key) == GLFW_RELEASE;
}

void Window::setKeyCallback(GLFWkeyfun callback) { glfwSetKeyCallback(mWindow, callback); } 
