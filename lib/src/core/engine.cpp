#include "jaroViewer/core/engine.hpp"
#include "jaroViewer/core/timer.hpp"
#include "jaroViewer/graphics/cubemap.hpp"
#include "jaroViewer/lighting/lightSet.hpp"

#include <memory>
#include <optional>

using namespace JaroViewer;

EngineState Engine::argsToState(const EngineArgs& args) {
	glfwInit();
	Window window{args.openGLMajor,  args.openGLMinor, args.windowWidth,
	              args.windowHeight, args.windowTitle, args.windowSamples};

	std::optional<Cubemap> mp = std::visit(
	  Tools::Overloaded{
	    [&](const std::string& dirPath) -> std::optional<Cubemap> {
		    if (dirPath == "") return std::nullopt;
		    return Cubemap(dirPath);
	    },
	    [&](const std::vector<std::string>& filePaths) -> std::optional<Cubemap> {
		    if (filePaths.size() == 0) return std::nullopt;
		    return Cubemap(filePaths);
	    },
	  },
	  args.cubemapParams
	);

	std::optional<PostProcessor> pp = (args.postProcessShader != "") ?
	  std::optional<PostProcessor>(PostProcessor(&window, args.postProcessShader)) :
	  std::nullopt;

	EngineState state{
	  std::move(window), Camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
	  std::move(mp), std::move(pp)
	};
	state.camera.addControls(state.input);
	return state;
}

Engine::Engine(const EngineArgs& args) : mState(argsToState(args)) {}
Engine::~Engine() { glfwTerminate(); }

void Engine::start() {
	mTransformUBO = std::make_shared<UniformBuffer>(0, sizeof(Tranformation), GL_STATIC_DRAW);
	mLightsUBO = std::shared_ptr<UniformBuffer>(
	  new UniformBuffer(1, sizeof(LightSet::LightSetStruct), GL_STATIC_DRAW)
	);
	render();
}

EngineState* Engine::getState() { return &mState; }

void Engine::render() {
	Tranformation trans{mState.window.getProjection(), mState.camera.getView()};
	Timer timer{};
	while (!mState.window.shouldClose()) {
		// Update the UBOs
		trans.view = mState.camera.getView();
		if (mState.window.updateView()) {
			if (mState.postProcessor)
				mState.postProcessor
				  ->resize(mState.window.getSize().width, mState.window.getSize().height);
			trans.projection = mState.window.getProjection();
		}
		mTransformUBO->updateData(&trans);
		LightSet::LightSetStruct lights = mState.lights.getStruct();
		mLightsUBO->updateData(&lights);

		// Process the inputs
		mState.input.processInputs(timer.getDeltaTime());

		// Redraw the screen
		mState.window.clear();
		if (mState.postProcessor)
			mState.postProcessor->bindAndClear(0.0f, 0.0f, 0.0f, 0.0f);

		mState.objectManager
		  .renderObjects(mState.postProcessor.has_value(), mState.camera.getPosition());
		if (mState.cubemap) mState.cubemap->render();

		if (mState.postProcessor) mState.postProcessor->render();
		mState.window.update();
	}
}
