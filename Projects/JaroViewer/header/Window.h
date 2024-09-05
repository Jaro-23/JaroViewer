#pragma once

#include <glad/glad.h>
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
			void setMouseMode(GLenum mode);

			bool shouldClose() const;
			int getMouseMode() const;

			GLFWwindow* getPointer() const;
			glm::mat4 getProjection() const;
			bool isKeyPressed(int key) const;
			bool isMouseKeyPressed(int mouseKey) const;
			bool isKeyReleased(int key) const;

			void setKeyCallback(GLFWkeyfun callback);

		private:
			GLFWwindow *mWindow;
			int mWidth, mHeight;
	};
}
