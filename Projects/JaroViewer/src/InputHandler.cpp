#include "../header/InputHandler.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace JaroViewer;

InputHandler::InputHandler(Window* window) :
	mWindow{window}
{
	addMouseCallback();	
}

void InputHandler::addKey(int key, KeyAction action, std::function<void(float)> func) {
	mKeys.insert(
		{key, 
		KeyState{
			func,
			mWindow->isKeyPressed(key),
			action,
			true
		}
	});
}

void InputHandler::addMouseKey(int key, KeyAction action, std::function<void(float)> func) {
	mKeys.insert(
		{key, 
		KeyState{
			func,
			mWindow->isKeyPressed(key),
			action,
			false
		}
	});
}

void InputHandler::addMouseMoveEvent(std::function<void(GLFWwindow*, double, double)> func) { mMouseMoveEvents.push_back(func); }

void InputHandler::processInputs(float deltaTime) {
	for (auto keyIter = mKeys.begin(); keyIter != mKeys.end(); keyIter++) {
		KeyState state = keyIter->second;
		bool pressed = (state.isKey) ? mWindow->isKeyPressed(keyIter->first) : mWindow->isMouseKeyPressed(keyIter->first);
		switch (state.action) {
			case KeyAction::DOWN:
				if (pressed) state.function(deltaTime);
				break;
			case KeyAction::UP:
				if (!pressed) state.function(deltaTime);
				break;
			case KeyAction::PRESS:
				if (pressed && !state.prevPressed) state.function(deltaTime);
				break;
			case KeyAction::RELEASE:
				if (!pressed && state.prevPressed) state.function(deltaTime);
				break;
		}
		keyIter->second.prevPressed = pressed;
	}
}

void InputHandler::addMouseCallback() {
	glfwSetWindowUserPointer(mWindow->getPointer(), this);
	glfwSetInputMode(mWindow->getPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(
		mWindow->getPointer(), 
		[](GLFWwindow *window, double xpos, double ypos) {
			InputHandler *handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
			for (int i = 0; i < handler->mMouseMoveEvents.size(); i++)
				handler->mMouseMoveEvents.at(i)(window, xpos, ypos);
		}
	);
}
