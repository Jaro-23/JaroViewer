#include "../header/InputHandler.h"

using namespace JaroViewer;

InputHandler::InputHandler(Window* window) :
	mWindow{window}
{

}

void InputHandler::addKey(int key, std::function<void(float)> lambda) {
	mKeys.insert({key, lambda});
}

void InputHandler::processInputs(float deltaTime) {
	for (auto keyFunc = mKeys.begin(); keyFunc != mKeys.end(); keyFunc++) {
		if (mWindow->isKeyPressed(keyFunc->first))
			keyFunc->second(deltaTime);
	}
}
