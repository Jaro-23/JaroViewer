#include "jaroViewer/core/window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace JaroViewer;

Window::Window(int major, int minor, int width, int height, const std::string& title)
  : Window(major, minor, width, height, title, 1) {}

Window::Window(int major, int minor, int width, int height, const std::string& title, uint samples)
  : mWidth(width), mHeight(height) {
	// TODO: Initialize glfw in engine
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (samples > 1) {
		glfwWindowHint(GLFW_SAMPLES, samples);
		mSamples = samples;
	}

	// Create base window
	mWindow = glfwCreateWindow(mWidth, mHeight, title.c_str(), NULL, NULL);
	if (mWindow == NULL) {
		glfwTerminate();
		std::cerr << "[Window] Error: Could not create a window named \'" + title + "\'"
		          << std::endl;
	}
	makeActive();

	// Initialize glad
	// TODO: Check if this can be done else where, because isn't logical (Example: engine after window creation)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		std::cerr << "[Window] Error: Could not initialize GLAD" << std::endl;
	}

	// Setup the viewport
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(mWindow, &fbWidth, &fbHeight);
	mWidth  = fbWidth;
	mHeight = fbHeight;
	glViewport(0, 0, mWidth, mHeight);
	glEnable(GL_DEPTH_TEST);
	if (samples > 0) glEnable(GL_MULTISAMPLE);

	clear();
	update();
}

Window::Window(Window&& other) noexcept
  : mWindow(other.mWindow),
    mWidth(other.mWidth),
    mHeight(other.mHeight),
    mWindowedWidth(other.mWindowedWidth),
    mWindowedHeight(other.mWindowedHeight),
    mPosX(other.mPosX),
    mPosY(other.mPosY),
    mSamples(other.mSamples),
    mIsFullscreen(other.mIsFullscreen) {
	other.mWindow = nullptr; // prevent the old object from destroying the window
}

Window::~Window() {
	if (mWindow) glfwDestroyWindow(mWindow);
}

void Window::clear() {
	GLFWwindow* current = glfwGetCurrentContext();
	if (current != mWindow) makeActive();

	int draw = 0, read = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, read);

	if (current != mWindow) glfwMakeContextCurrent(current);
}

void Window::update() {
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

bool Window::updateView() {
	int width, height;
	glfwGetFramebufferSize(mWindow, &width, &height);
	if (mWidth == width && mHeight == height) return false;

	mWidth  = width;
	mHeight = height;
	glViewport(0, 0, width, height);
	return true;
}

void Window::toggleFullscreen() {
	if (!mIsFullscreen) {
		glfwGetWindowPos(mWindow, &mPosX, &mPosY);
		glfwGetWindowSize(mWindow, &mWindowedWidth, &mWindowedHeight);
		GLFWmonitor* monitor    = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(mWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	} else {
		glfwSetWindowMonitor(mWindow, nullptr, mPosX, mPosY, mWindowedWidth, mWindowedHeight, 0);
	}
	glfwGetWindowSize(mWindow, &mWidth, &mHeight);
	mIsFullscreen = !mIsFullscreen;
}

void Window::setShouldClose(bool close) {
	glfwSetWindowShouldClose(mWindow, close);
}

void Window::setMouseMode(GLenum mode) {
	glfwSetInputMode(mWindow, GLFW_CURSOR, mode);
}

void Window::makeActive() { glfwMakeContextCurrent(mWindow); }

bool Window::insideScreen(int x, int y) const {
	return (0 <= x && x <= mWidth && 0 <= y && y <= mHeight);
}

bool Window::shouldClose() const { return glfwWindowShouldClose(mWindow); }

int Window::getMouseMode() const {
	return glfwGetInputMode(mWindow, GLFW_CURSOR);
}

Size Window::getSize() const { return {mWidth, mHeight}; }

GLFWwindow* Window::cPtr() const { return mWindow; }

glm::mat4 Window::getProjection() const {
	return glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
}
