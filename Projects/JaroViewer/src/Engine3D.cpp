#include "../header/Engine3D.h"
#include "../header/Timer.h"

#include <GLM/gtx/string_cast.hpp>
#include <algorithm>
#include <memory>

using namespace JaroViewer;

Engine3D::Engine3D(const Window &window, Camera* camera) :
	mWindow{window},
	mInputHandler{&mWindow},
	mCamera{camera}
{
	mCamera->addControls(&mInputHandler);
}

/**
 * Starts the engine and opens the window
 */
void Engine3D::start() {
	mTransformationUBO = std::shared_ptr<UniformBuffer>( new UniformBuffer(0, sizeof(UniformTransformation), GL_STATIC_DRAW) );
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
	mLightSetUBO = std::shared_ptr<UniformBuffer>( new UniformBuffer(1, sizeof(LightSet::LightSetStruct), GL_STATIC_DRAW) );
}

InputHandler* Engine3D::getInputHandler() { return &mInputHandler; }

/**
 * Creates a postprocessor with the given fragment shader
 * @param fragmentPath The path to the fragment shader
 */
void Engine3D::enablePostProcessor(const std::string fragmentPath) {
	mPostProcessor = std::make_unique<PostProcessor>(mWindow.getWidth(), mWindow.getHeight(), fragmentPath);
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
		mTransformationUBO->UpdateData(&uniformData);
		LightSet::LightSetStruct lightStruct = mLightSet->getStruct();
		mLightSetUBO->UpdateData(&lightStruct);

		if (mWindow.updateView()) uniformData.projection = mWindow.getProjection();
		mInputHandler.processInputs(data.deltaTime);
		updateFrame(data);		
	}
}

/**
 * Updates the frame
 * @param data The data that will be given to each component
 */
void Engine3D::updateFrame(const Component3D::RenderData &data) {
	mWindow.clear();

	if (mPostProcessor) {
		mPostProcessor->bind();
		mPostProcessor->clear(0.0f, 0.0f, 0.0f, 0.0f);
	}

	std::map<int, std::shared_ptr<JaroViewer::Component3D>>::iterator it;
	for (it = mComponents.begin(); it != mComponents.end(); it++) it->second->render(data);

	if (mPostProcessor) {
		mPostProcessor->render();
	}

	mWindow.update();
}
