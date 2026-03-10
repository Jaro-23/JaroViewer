#include "../headers/engine.hpp"
#include "../headers/timer.hpp"
#include <memory>

using namespace JaroViewer;

EngineState Engine::argsToState(const EngineArgs& args) {
	glfwInit();
	Window window{args.openGLMajor,  args.openGLMinor, args.windowWidth,
	              args.windowHeight, args.windowTitle, args.windowSamples};

	EngineState state{std::move(window), Camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f))};
	state.camera.addControls(state.input);
	return state;
}

Engine::Engine(const EngineArgs& args) : mState(argsToState(args)) {}
Engine::~Engine() { glfwTerminate(); }

void Engine::start() {
	mTransformUBO = std::make_shared<UniformBuffer>(0, sizeof(Tranformation), GL_STATIC_DRAW);
	render();
}

EngineState* Engine::getState() { return &mState; }

void Engine::render() {
	Tranformation trans{mState.window.getProjection(), mState.camera.getView()};
	Timer timer{};
	while (!mState.window.shouldClose()) {
		// Update the transformation UBO
		trans.view = mState.camera.getView();
		if (mState.window.updateView())
			trans.projection = mState.window.getProjection();
		mTransformUBO->updateData(&trans);

		// Process the inputs
		mState.input.processInputs(timer.getDeltaTime());

		// Redraw the screen
		mState.window.clear();
		mState.objectManager.renderObjects();
		mState.window.update();
	}
}
