#include "Spotlight.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <cmath>
#include <memory>

namespace JaroViewer {
	class Camera {
		public:
			static void setupCallback(GLFWwindow *window, Camera *camera);

			Camera(glm::vec3 pos, glm::vec3 up);
			void setFlashlight(const std::shared_ptr<JaroViewer::Spotlight> flashlight);
			void toggleFlashlight();

			void goForward(float deltaTime);
			void goBack(float deltaTime);
			void goLeft(float deltaTime);
			void goRight(float deltaTime);
			void updateDirection(float yaw, float pitch);

			glm::mat4 getView() const;
			glm::vec3 getPosition() const;
			void setSpeed(float speed);
			bool getFirstMove() const;
			void setFirstMove(bool state);
			float getMouseX() const;
			float getMouseY() const;
			void setMouseX(float x);
			void setMouseY(float y);
			float getSensitivity() const;

		private:
			std::shared_ptr<JaroViewer::Spotlight> mFlashlight;
			bool mEnableFlashlight;

			// Lateral freedom
			glm::vec3 mPos;
			glm::vec3 mFront;
			glm::vec3 mUp;

			float mSpeed = 0.5f;

			// Rotational freedom
			float mYaw = -90.0f;
			float mPitch = 0.0f;

			float mMouseX = 0.0f;
			float mMouseY = 0.0f;
			bool mFirstMove = true;

			float mSensitivity = 0.07f;
	};
}
