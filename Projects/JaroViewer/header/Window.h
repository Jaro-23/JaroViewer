#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include <string>

namespace JaroViewer {
	class Window {
		public:
			Window(int glfwVersion, int width, int height, const std::string &title);
			Window(int glfwVersion, int width, int height, const std::string &title, unsigned int samples);

			void clear();
			void update();
			bool updateView();
			void toggleFullscreen();
			
			void setShouldClose(bool close);
			void setMouseMode(GLenum mode);

			bool shouldClose() const;
			int getMouseMode() const;
			int getWidth() const;
			int getHeight() const;

			GLFWwindow* getPointer() const;
			glm::mat4 getProjection() const;
			bool isKeyPressed(int key) const;
			bool isMouseKeyPressed(int mouseKey) const;
			bool isKeyReleased(int key) const;

			void setKeyCallback(GLFWkeyfun callback);

		private:
			GLFWwindow *mWindow;
			int mWidth, mHeight;
			int mWindowedWidth, mWindowedHeight;
			int mPosX, mPosY;
			unsigned int mSamples;
			bool mIsFullscreen;
	};
}
