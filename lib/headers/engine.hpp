#pragma once

#include "camera.hpp"
#include "inputHandler.hpp"
#include "lightSet.hpp"
#include "objectManager.hpp"
#include "uniformBuffer.hpp"
#include "window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

namespace JaroViewer {
	struct EngineArgs {
		// Window args
		int openGLMajor         = 4;
		int openGLMinor         = 6;
		int windowWidth         = 600;
		int windowHeight        = 450;
		std::string windowTitle = "Basic window";
		uint windowSamples      = 1;
	};

	struct EngineState {
		Window window;
		Camera camera;
		InputHandler input;
		ObjectManager objectManager;
		LightSet lights;

		EngineState(Window&& w, Camera c)
		  : window(std::move(w)),
		    camera(std::move(c)),
		    input(&this->window),
		    objectManager(),
		    lights() {}
	};

	class Engine {
	public:
		static EngineState argsToState(const EngineArgs& args);
		Engine(const EngineArgs& args);
		~Engine();

		void start();
		EngineState* getState();

	private:
		struct Tranformation {
			glm::mat4 projection;
			glm::mat4 view;
		};

		void render();

		EngineState mState;
		std::shared_ptr<UniformBuffer> mTransformUBO;
		std::shared_ptr<UniformBuffer> mLightsUBO;
	};
} // namespace JaroViewer
