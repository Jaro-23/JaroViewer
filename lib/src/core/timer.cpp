#include "core/timer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace JaroViewer;

Timer::Timer() { mLastTime = 0.0f; }

float Timer::getDeltaTime() {
	float curr  = glfwGetTime();
	float delta = curr - mLastTime;
	mLastTime   = curr;
	return delta;
}
