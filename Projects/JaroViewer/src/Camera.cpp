#include "../header/Camera.h"
#include "GLFW/glfw3.h"

#include <cmath>
#include <memory>

using namespace JaroViewer;

Camera::Camera(glm::vec3 pos, glm::vec3 up) :
	mPos{pos},
	mUp{up},
	mFlashlight{std::shared_ptr<Spotlight>(nullptr)}
{
	updateDirection(0.0f, 0.0f);
}

/**
 * Sets a spotlight to the camera to function as flashlight
 * @param flashlight The spotlight that will become the flashlight
 */
void Camera::setFlashlight(const std::shared_ptr<Spotlight> flashlight) { 
	mFlashlight = flashlight; 
	mFlashlight->setTranslation(mPos);
	mFlashlight->setDirection(mFront);
}

/**
 * Toggles the flashlight on and off
 */
void Camera::toggleFlashlight() { if (mFlashlight) mFlashlight->enable(!mFlashlight->getState()); }

/**
 * Add all camera controls for this camera to the input handler
 * @param handler The input handler
 */
void Camera::addControls(InputHandler* handler) {
	// Basic movement
	handler->addMouseMoveEvent(getMouseEvent());
	handler->addKey(GLFW_KEY_W, InputHandler::KeyAction::DOWN, [=] (float deltaTime) { this->goForward(deltaTime); });
	handler->addKey(GLFW_KEY_S, InputHandler::KeyAction::DOWN, [=] (float deltaTime) { this->goBack(deltaTime); });
	handler->addKey(GLFW_KEY_A, InputHandler::KeyAction::DOWN, [=] (float deltaTime) { this->goLeft(deltaTime); });
	handler->addKey(GLFW_KEY_D, InputHandler::KeyAction::DOWN, [=] (float deltaTime) { this->goRight(deltaTime); });

	// Flashlight
	handler->addKey(GLFW_KEY_F, InputHandler::KeyAction::PRESS, [=] (float) { this->toggleFlashlight(); });

	// Set focus keys
	handler->addKey(GLFW_KEY_ESCAPE, InputHandler::KeyAction::DOWN, [=] (float) { 
		if(handler->getWindow()->getMouseMode() == GLFW_CURSOR_DISABLED) handler->getWindow()->setMouseMode(GLFW_CURSOR_NORMAL); 
	});
	handler->addMouseKey(GLFW_MOUSE_BUTTON_LEFT, InputHandler::KeyAction::PRESS, [=](float) {
		if(handler->getWindow()->getMouseMode() == GLFW_CURSOR_NORMAL) handler->getWindow()->setMouseMode(GLFW_CURSOR_DISABLED); 
	});
}

/**
 * Moves to camera forward
 * @param deltatime The time since last move for constant movement
 */
void Camera::goForward(float deltaTime) { 
	mPos += mFront * mSpeed * deltaTime; 
	if (mFlashlight) mFlashlight->setTranslation(mPos);
}

/**
 * Moves to camera backward
 * @param deltatime The time since last move for constant movement
 */
void Camera::goBack(float deltaTime) {
	mPos -= mFront * mSpeed * deltaTime; 
	if (mFlashlight) mFlashlight->setTranslation(mPos);
}

/**
 * Moves to camera to the left
 * @param deltatime The time since last move for constant movement
 */
void Camera::goLeft(float deltaTime) {
	mPos -= glm::normalize(glm::cross(mFront, mUp)) * mSpeed * deltaTime; 
	if (mFlashlight) mFlashlight->setTranslation(mPos);
}

/**
 * Moves to camera to the right
 * @param deltatime The time since last move for constant movement
 */
void Camera::goRight(float deltaTime) {
	mPos += glm::normalize(glm::cross(mFront, mUp)) * mSpeed * deltaTime; 
	if (mFlashlight) mFlashlight->setTranslation(mPos);
}

/**
 * Updates the direction of the camera
 * @param yaw The new yaw of the camera
 * @param pitch The new pitch of the camera
 */
void Camera::updateDirection(float yaw, float pitch) {
	mYaw += yaw;
	mPitch += pitch;

	if (mPitch >  89.0f) mPitch =  89.0f;
	if (mPitch < -89.0f) mPitch = -89.0f;

	mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront.y = sin(glm::radians(mPitch));
	mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(mFront);
	if (mFlashlight) mFlashlight->setDirection(mFront);
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

/**
 * Returns the mouse move event for the mouse
 */
std::function<void(GLFWwindow*, double, double)> Camera::getMouseEvent() {
	return [=](GLFWwindow *window, double xpos, double ypos) {
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) return;
		if (this->getFirstMove()) {
			this->setMouseX(xpos);
			this->setMouseY(ypos);
			this->setFirstMove(false);
			return;
		}

		float xoffset = xpos - this->getMouseX();
		float yoffset = this->getMouseY() - ypos;
		this->setMouseX(xpos);
		this->setMouseY(ypos);

		xoffset *= this->getSensitivity();
		yoffset *= this->getSensitivity();

		this->updateDirection(xoffset, yoffset);
	};
}
