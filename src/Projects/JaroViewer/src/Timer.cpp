#include "../header/Timer.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

using namespace JaroViewer;

Timer::Timer() {
	mLastTime = 0.0f;
}

/**
 * Returns the time since the last time this function was called
 */
float Timer::getDeltaTime() {
	float curr = glfwGetTime();
	float delta = curr - mLastTime;
	mLastTime = curr;
	return delta;
}
