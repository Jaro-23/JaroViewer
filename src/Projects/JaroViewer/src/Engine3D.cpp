#include "../header/Engine3D.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <iostream>

using namespace JaroViewer;

Engine3D::Engine3D(int glfwVersion, int width, int height, const char* title, Camera* camera) :
	mWidth{width},
	mHeight{height},
	mComponents{},
	mComponentsLength{0},
	mOpenSlots{},
	mCamera{camera}
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

	// Setting up the viewport
	glViewport(0, 0, mWidth, mHeight);
	glEnable(GL_DEPTH_TEST);
	Camera::setupCallback(mWindow, mCamera);
}

/**
 * Starts the engine and opens the window
 */
void Engine3D::start() {
	// TODO: do the setup
	// Setup the ubo
	glGenBuffers(1, &mUboBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, mUboBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, mUboBuffer, 0, 2 * sizeof(glm::mat4));

	render();
	glfwTerminate();
}

/**
 * Adds a component to the rendering buffer
 * @param component The component that is to be added
 * @return The id of where the component is stored
 */
unsigned int Engine3D::addComponent(JaroViewer::Component3D* component) {
	int index = 0;
	if (mOpenSlots.empty()) {
		mComponents.insert({mComponentsLength, component});
		index = mComponentsLength++;
	} else {
		index = mOpenSlots.back();
		mOpenSlots.pop_back();
		mComponents.at(index) = component;
	}
	component->load();
	return index;
}

/**
 * Returns a pointer to a component
 * @param id The id of the component to be retreived
 * @return A pointer to the component
 */
Component3D* Engine3D::getComponent(unsigned int id) {
	if (id >= mComponentsLength || 0 < std::count(mOpenSlots.begin(), mOpenSlots.end(), id)) return nullptr;
	return mComponents.at(id);
}

/**
 * Removes the component with certain id from the rendering buffer
 * @param id The id of the component needed to be removed
 * @post The component will be deleted
 */
void Engine3D::removeComponent(unsigned int id) {
	if (id >= mComponentsLength || 0 < std::count(mOpenSlots.begin(), mOpenSlots.end(), id)) return;
	mComponents.erase(id);	
}

/**
 * Contains the main rendering loop 
 */
void Engine3D::render() {
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;
	while (!glfwWindowShouldClose(mWindow)) {
		deltaTime = glfwGetTime() - lastFrame;
		lastFrame = deltaTime;

		int width, height;
		glfwGetWindowSize(mWindow, &width, &height);
		if (mWidth != width || mHeight != height) {
			mWidth = width;
			mHeight = height;
			glViewport(0, 0, width, height);
		}

		Component3D::RenderData data{
			glm::mat4(1.0f),
			mCamera->getView(),
			glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f)
		};

		processInput(mWindow, deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		std::map<int, JaroViewer::Component3D*>::iterator it;
		for (it = mComponents.begin(); it != mComponents.end(); it++) {
			it->second->render(data);
		}

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

void Engine3D::processInput(GLFWwindow *window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (mCamera) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) mCamera->goForward(deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) mCamera->goBack(deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) mCamera->goLeft(deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) mCamera->goRight(deltaTime);
	}
}
