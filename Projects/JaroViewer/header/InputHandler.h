#pragma once

#include "Window.h"

#include <functional>
#include <map>

namespace JaroViewer {
	class InputHandler {
		public:
			InputHandler(Window* window);
			void addKey(int key, std::function<void(float)> lambda);
			void processInputs(float deltaTime);

		private:
			Window* mWindow;
			std::map<int, std::function<void(float)>> mKeys;
	};
}
