#include "../header/Engine3D.h"
#include "../header/Tools.h"
#include "../header/Timer.h"

#include <GLM/gtx/string_cast.hpp>
#include <algorithm>
#include <iostream>
#include <memory>

using namespace JaroViewer;

Engine3D::Engine3D(const Window &window, Camera* camera) :
	mComponents{},
	mComponentsLength{0},
	mOpenSlots{},
	mWindow{window},
	mCamera{camera},
	mLightSet{nullptr}
{
	Camera::setupCallback(window.getPointer(), mCamera);
}

/**
 * Starts the engine and opens the window
 */
void Engine3D::start() {
	// Setup the ubo
	mTransformationUBO = Tools::generateUniformBuffer(0, sizeof(UniformTransformation), GL_STATIC_DRAW);

	render();
	glfwTerminate();
}

/**
 * Adds a component to the rendering buffer
 * @param component The component that is to be added
 * @return The id of where the component is stored
 */
unsigned int Engine3D::addComponent(std::shared_ptr<JaroViewer::Component3D> component) {
	int index;
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
std::shared_ptr<JaroViewer::Component3D> Engine3D::getComponent(unsigned int id) {
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

void Engine3D::setLightSet(LightSet* lightSet) { 
	mLightSet = lightSet; 
	mLightSetUBO = Tools::generateUniformBuffer(1, sizeof(LightSet::LightSetStruct), GL_STATIC_DRAW);
}

/**
 * Contains the main rendering loop 
 */
void Engine3D::render() {
	UniformTransformation uniformData {
		mWindow.getProjection(),
		mCamera->getView()
	};

	// Start the main loop
	Timer timer{};
	while (!mWindow.shouldClose()) {
		Component3D::RenderData data{
			mCamera->getPosition(),
			timer.getDeltaTime()	
		};
		
		uniformData.view = mCamera->getView();
		Tools::updateUniformBufferData(mTransformationUBO, &uniformData, sizeof(uniformData));
		LightSet::LightSetStruct lightStruct = mLightSet->getStruct();
		Tools::updateUniformBufferData(mLightSetUBO, &lightStruct, sizeof(lightStruct));

		if (mWindow.updateView())
			uniformData.projection = mWindow.getProjection();

		processInput(data.deltaTime);
		updateFrame(data);		
	}
}

/**
 * Reads and act on input queues
 * @param deltaTime The time since the last frame
 */
void Engine3D::processInput(float deltaTime) {
	if (mWindow.isKeyPressed(GLFW_KEY_ESCAPE)) mWindow.setShouldClose(true);

	if (mCamera) {
		if (mWindow.isKeyPressed(GLFW_KEY_W)) mCamera->goForward(deltaTime);
		if (mWindow.isKeyPressed(GLFW_KEY_S)) mCamera->goBack(deltaTime);
		if (mWindow.isKeyPressed(GLFW_KEY_A)) mCamera->goLeft(deltaTime);
		if (mWindow.isKeyPressed(GLFW_KEY_D)) mCamera->goRight(deltaTime);
		if (mWindow.isKeyPressed(GLFW_KEY_F)) mCamera->toggleFlashlight();
	}
}


/**
 * Updates the frame
 * @param data The data that will be given to each component
 */
void Engine3D::updateFrame(const Component3D::RenderData &data) {
	mWindow.clearWindow();

	std::map<int, std::shared_ptr<JaroViewer::Component3D>>::iterator it;
	for (it = mComponents.begin(); it != mComponents.end(); it++) it->second->render(data);

	mWindow.update();
}
