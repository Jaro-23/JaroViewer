#pragma once

#include "Camera.hpp"
#include "InputHandler.hpp"
#include "UniformBuffer.hpp"
#include "Window.hpp"

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

		EngineState(Window&& w, Camera c)
		  : window(std::move(w)), camera(std::move(c)), input(&this->window) {}
	};

	class Engine {
	public:
		static EngineState argsToState(const EngineArgs& args);
		Engine(const EngineArgs& args);
		~Engine();

		void start();

	private:
		struct Tranformation {
			glm::mat4 projection;
			glm::mat4 view;
		};

		void render();

		EngineState mState;
		std::shared_ptr<UniformBuffer> mTransformUBO;
	};
} // namespace JaroViewer
