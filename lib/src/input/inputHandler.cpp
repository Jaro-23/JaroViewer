#include "input/inputHandler.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace JaroViewer;

InputHandler::InputHandler(Window* window) : mWindow{window} {
	addMouseCallback();
}

Window* InputHandler::getWindow() const { return mWindow; }

void InputHandler::addKey(int key, KeyAction action, std::function<void(float)> func) {
	mKeys.insert({key, KeyState{func, glfwGetKey(mWindow->cPtr(), key) == GLFW_PRESS, action, true}});
}

void InputHandler::addMouseKey(int key, KeyAction action, std::function<void(float)> func) {
	mKeys.insert(
	  {key, KeyState{func, glfwGetMouseButton(mWindow->cPtr(), key) == GLFW_PRESS, action, false}}
	);
}

void InputHandler::addMouseMoveEvent(std::function<void(GLFWwindow*, double, double)> func) {
	mMouseMoveEvents.push_back(func);
}

void InputHandler::processInputs(float deltaTime) {
	for (auto keyIter = mKeys.begin(); keyIter != mKeys.end(); keyIter++) {
		KeyState state = keyIter->second;
		bool pressed   = (state.isKey) ?
		    glfwGetKey(mWindow->cPtr(), keyIter->first) == GLFW_PRESS :
		    glfwGetMouseButton(mWindow->cPtr(), keyIter->first) == GLFW_PRESS;
		switch (state.action) {
		case KeyAction::DOWN:
			if (pressed) state.function(deltaTime);
			break;
		case KeyAction::UP:
			if (!pressed) state.function(deltaTime);
			break;
		case KeyAction::PRESS:
			if (pressed && !state.isPressed) state.function(deltaTime);
			break;
		case KeyAction::RELEASE:
			if (!pressed && state.isPressed) state.function(deltaTime);
			break;
		}
		keyIter->second.isPressed = pressed;
	}
}

void InputHandler::addMouseCallback() {
	glfwSetWindowUserPointer(mWindow->cPtr(), this);
	glfwSetInputMode(mWindow->cPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(mWindow->cPtr(), [](GLFWwindow* window, double xpos, double ypos) {
		InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
		for (size_t i = 0; i < handler->mMouseMoveEvents.size(); ++i)
			handler->mMouseMoveEvents.at(i)(window, xpos, ypos);
	});
}
