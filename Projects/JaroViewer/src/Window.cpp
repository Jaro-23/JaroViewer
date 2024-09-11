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
Window::Window(int glfwVersion, int width, int height, const std::string &title) : Window{glfwVersion, width, height, title, 1} {}

/**
 * Creates a window
 * @param glfwVersion The version of glfw that will be used
 * @param width The width of the screen
 * @param height The height of the screen
 * @param title The title of the window
 * @param samples The number of samples for anti-aliasing
 */
Window::Window(int glfwVersion, int width, int height, const std::string &title, unsigned int samples) :
	mWidth{width},
	mHeight{height},
	mIsFullscreen{false}
{
	// Start up GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (samples > 1) {
		glfwWindowHint(GLFW_SAMPLES, samples);
		mSamples = samples;
	} else {
		mSamples = 1;
	}

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
	if (samples > 0) glEnable(GL_MULTISAMPLE);

	clear();
	update();
}

/**
 * Clears the window
 * @post the window will be fully black
 */
void Window::clear() {
	int draw = 0, read = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, read);
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
 * 
 */
void Window::toggleFullscreen() {
	if (!mIsFullscreen) {
		glfwGetWindowPos(mWindow, &mPosX, &mPosY);
		glfwGetWindowSize(mWindow, &mWindowedWidth, &mWindowedHeight);
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(mWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	} else {
		glfwSetWindowMonitor(mWindow, nullptr, mPosX, mPosY, mWindowedWidth, mWindowedHeight, 0);
	}
	glfwGetWindowSize(mWindow, &mWidth, &mHeight);
	mIsFullscreen = !mIsFullscreen;
}

/**
 * Sets the should close variable
 * @param close The new value of the should close variable
 */
void Window::setShouldClose(bool close) { glfwSetWindowShouldClose(mWindow, close); }

/**
 * Sets the mouse mode
 * @param mode The new mode for the mouse
 */
void Window::setMouseMode(GLenum mode) { glfwSetInputMode(mWindow, GLFW_CURSOR, mode); }

/**
 * Returns the should close variable
 * @return The value of the should close variable
 */
bool Window::shouldClose() const { return glfwWindowShouldClose(mWindow); }

/**
 * Gets the cursor input mode
 */
int Window::getMouseMode() const { return glfwGetInputMode(mWindow, GLFW_CURSOR); }

int Window::getWidth() const { return mWidth; }
int Window::getHeight() const { return mHeight; }

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
 * Returns if a mouse key is pressed on the window
 * @return True if the mouse key is pressed otherwise false
 */
bool Window::isMouseKeyPressed(int key) const {
	return glfwGetMouseButton(mWindow, key) == GLFW_PRESS;
}

/**
 * Returns if a key is released on the window
 * @return True if the key is pressed otherwise false
 */
bool Window::isKeyReleased(int key) const {
	return glfwGetKey(mWindow, key) == GLFW_RELEASE;
}

void Window::setKeyCallback(GLFWkeyfun callback) { glfwSetKeyCallback(mWindow, callback); } 
