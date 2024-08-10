#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include <string>

namespace JaroViewer {
	class Window {
		public:
			Window(int glfwVersion, int width, int height, const std::string &title);

			void clearWindow();
			void update();
			bool updateView();
			
			void setShouldClose(bool close);
			bool shouldClose() const;

			GLFWwindow* getPointer() const;
			glm::mat4 getProjection() const;
			bool isKeyPressed(int key) const;

		private:
			GLFWwindow *mWindow;
			int mWidth, mHeight;
	};
}
