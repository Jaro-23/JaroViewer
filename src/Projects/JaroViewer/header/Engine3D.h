#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Component3D.h"
#include "Camera.h"

namespace JaroViewer {
		class Engine3D {
		public:			
			Engine3D(int glfwVersion, int width, int height, const char* title, Camera* camera);

			void start();
			unsigned int addComponent(JaroViewer::Component3D* component);
			JaroViewer::Component3D* getComponent(unsigned int id);
			void removeComponent(unsigned int id);

		private:
			void render();
			void processInput(GLFWwindow *window, float deltaTime);

			struct UniformMatrices {
				glm::mat4 projection;
				glm::mat4 view;
			};

			int mWidth;
			int mHeight;

			std::map<int, JaroViewer::Component3D*> mComponents;
			int mComponentsLength;
			std::vector<int> mOpenSlots;

			GLFWwindow *mWindow;
			Camera* mCamera;
			unsigned int mUboBuffer;
	};
};

