#pragma once

#include "Window.h"

#include <functional>
#include <map>
#include <vector>

namespace JaroViewer {
	class InputHandler {
		public:
			enum class KeyAction {
				DOWN,
				UP,
				PRESS,
				RELEASE
			};

			InputHandler(Window* window);
			void addKey(int key, KeyAction action, std::function<void(float)> func);
			void addMouseEvent(std::function<void(GLFWwindow*, double, double)> func);
			void processInputs(float deltaTime);

		private:
			void addMouseCallback();

			struct KeyState {
				std::function<void(float)> function;
				int prevPressed;
				KeyAction action;
			};

			Window* mWindow;
			std::map<int, KeyState> mKeys;
			std::vector<std::function<void(GLFWwindow*, double, double)>> mMouseEvents;
	};
}
