#include "../header/InputHandler.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace JaroViewer;

InputHandler::InputHandler(Window* window) :
	mWindow{window}
{
	addMouseCallback();	
}

/**
 * Returns the window on which the handler is active
 */
Window* InputHandler::getWindow() const { return mWindow; }

/**
 * Adds a key event
 * @param key The key on which the action will trigger
 * @param action What needs to happen to the key to trigger the event
 * @param func The event that will be called
 */
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

/**
 * Adds a mouse key event
 * @param key The mouse key on which the action will trigger
 * @param action What needs to happen to the mouse key to trigger the event
 * @param func The event that will be called
 */
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

/**
 * Adds a mouse move event
 * @param func The event that will be triggered when the mouse moves
 */
void InputHandler::addMouseMoveEvent(std::function<void(GLFWwindow*, double, double)> func) { mMouseMoveEvents.push_back(func); }

/**
 * Checks all the event triggers and triggers events when passes the check
 * @param deltaTime The time since the last call
 */
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

/**
 * Adds the callback function to the window
 */
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
