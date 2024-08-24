#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <memory>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Component3D.h"
#include "Camera.h"
#include "Window.h"
#include "LightSet.h"

namespace JaroViewer {
		class Engine3D {
		public:			
			Engine3D(const Window &window, Camera* camera);

			void start();
			unsigned int addComponent(std::shared_ptr<JaroViewer::Component3D> component);
			std::shared_ptr<JaroViewer::Component3D> getComponent(unsigned int id);
			void removeComponent(unsigned int id);

			void setLightSet(LightSet* lightSet);

		private:
			void render();
			void processInput(float deltaTime);

			struct UniformTransformation {
				glm::mat4 projection;
				glm::mat4 view;
			};
			void updateView(UniformTransformation &uniformData);
			void updateFrame(const Component3D::RenderData &data);
			
			std::map<int, std::shared_ptr<JaroViewer::Component3D>> mComponents;
			int mComponentsLength;
			std::vector<int> mOpenSlots;

			Window mWindow;
			Camera* mCamera;
			LightSet* mLightSet;	

			unsigned int mTransformationUBO;
			unsigned int mLightSetUBO;
	};
};
