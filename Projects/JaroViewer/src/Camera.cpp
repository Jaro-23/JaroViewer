#include "../header/Camera.h"

#include <cmath>
#include <iostream>

using namespace JaroViewer;

void Camera::setupCallback(GLFWwindow *window, Camera *camera) {
	glfwSetWindowUserPointer(window, camera);
	auto func = [](GLFWwindow *window, double xpos, double ypos) {
		Camera *camObj = static_cast<Camera*>(glfwGetWindowUserPointer(window));
		if (!camObj) return;
		if (camObj->getFirstMove()) {
			camObj->setMouseX(xpos);
			camObj->setMouseY(ypos);
			camObj->setFirstMove(false);
			return;
		}

		float xoffset = xpos - camObj->getMouseX();
		float yoffset = camObj->getMouseY() - ypos;
		camObj->setMouseX(xpos);
		camObj->setMouseY(ypos);

		xoffset *= camObj->getSensitivity();
		yoffset *= camObj->getSensitivity();

		camObj->updateDirection(xoffset, yoffset);
	};
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, func);
}

Camera::Camera(glm::vec3 pos, glm::vec3 up) {
	mPos = pos;
	mUp = up;
	updateDirection(0.0f, 0.0f);
}

void Camera::goForward(float deltaTime) { mPos += mFront * mSpeed * deltaTime; }
void Camera::goBack(float deltaTime) { mPos -= mFront * mSpeed * deltaTime; }
void Camera::goLeft(float deltaTime) { mPos -= glm::normalize(glm::cross(mFront, mUp)) * mSpeed * deltaTime; }
void Camera::goRight(float deltaTime) { mPos += glm::normalize(glm::cross(mFront, mUp)) * mSpeed * deltaTime; }
void Camera::updateDirection(float yaw, float pitch) {
	mYaw += yaw;
	mPitch += pitch;

	if (mPitch >  89.0f) mPitch =  89.0f;
	if (mPitch < -89.0f) mPitch = -89.0f;

	mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront.y = sin(glm::radians(mPitch));
	mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(mFront);
}

void Camera::setSpeed(float speed) { mSpeed = speed; }
glm::mat4 Camera::getView() const { return glm::lookAt(mPos, mPos + mFront, mUp); }
glm::vec3 Camera::getPosition() const { return mPos; }
bool Camera::getFirstMove() const { return mFirstMove; }
void Camera::setFirstMove(bool state) { mFirstMove = state; }
float Camera::getMouseX() const { return mMouseX; }
float Camera::getMouseY() const { return mMouseY; }
void Camera::setMouseX(float x) { mMouseX = x; }
void Camera::setMouseY(float y) { mMouseY = y; }
float Camera::getSensitivity() const { return mSensitivity; }
