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
#include "PostProcessor.h"
#include "Cubemap.h"
#include "UniformBuffer.h"
#include "Window.h"
#include "LightSet.h"

namespace JaroViewer {
		class Engine3D {
		public:			
			typedef struct {
				std::shared_ptr<PostProcessor> postProcessor;
				std::shared_ptr<Cubemap> cubemap;
			} EngineArgs;

			Engine3D(Window* window, Camera* camera);

			void start();

			unsigned int addComponent(std::shared_ptr<JaroViewer::Component3D> component);
			std::shared_ptr<JaroViewer::Component3D> getComponent(unsigned int id);
			void removeComponent(unsigned int id);

			void setLightSet(LightSet* lightSet);
			InputHandler* getInputHandler();
			void setArgs(EngineArgs args);
			EngineArgs* getArgsPtr();

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

			Window* mWindow;
			Camera* mCamera;
			LightSet* mLightSet;	
			InputHandler mInputHandler;
			EngineArgs mArgs;

			std::shared_ptr<UniformBuffer> mTransformationUBO;
			std::shared_ptr<UniformBuffer> mLightSetUBO;
	};
};

