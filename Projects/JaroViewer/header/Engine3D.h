#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <memory>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Component3D.h"
#include "Camera.h"
#include "InputHandler.h"
#include "UniformBuffer.h"
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
			InputHandler* getInputHandler();

		private:
			void render();

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
			InputHandler mInputHandler;

			std::shared_ptr<UniformBuffer> mTransformationUBO;
			std::shared_ptr<UniformBuffer> mLightSetUBO;
	};
};

