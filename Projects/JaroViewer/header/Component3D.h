#pragma once

#include "Material.h"
#include "Shader.h"
#include <GLM/glm.hpp>
#include <memory>
#include <vector>

namespace JaroViewer {
	class Component3D {
		public:
			// Defining basic communications strcutures for the component		
			typedef struct RenderData {
				glm::vec3 viewPos;
				float deltaTime;
			} RenderData;

			Component3D(const Shader &shader);
			void setWireframeMode(bool enable);
			
			void addTranslation(const glm::vec3 &translation);
			void addRotation(float angleX, float angleY, float angleZ);
			void addScale(const glm::vec3 &scale);
			void addScale(float scale);

			void setTranslation(const glm::vec3 &translation);
			void setRotation(float angleX, float angleY, float angleZ);
			void setScale(const glm::vec3 &scale);
			void setScale(float scale);

			void addComponent(std::shared_ptr<Component3D> &component);
			void addMaterial(const Material &material);

			virtual void load() = 0;
			void render(const RenderData &data);

		protected:
			glm::mat4 getModelMatrix() const;
			glm::mat3 getNormalModelMatrix(const glm::mat4 &model) const;
			glm::vec3 getPosition() const;
			void setUseIndices(bool use);
			void bindMaterials(const Shader &shader) const;

			unsigned int mNumLines = 0;
			unsigned int mVaoBuffer = 0;
			Shader mShader;

		private:
			void wireframeRender(const RenderData &data);
			virtual void defaultRender(const RenderData &data);
			void normalizeAngles();

			Shader mWireframeShader;
			bool mWireframeMode;
			bool mUseIndices;
			std::vector<std::shared_ptr<Component3D>> mChildren;

			std::vector<JaroViewer::Material> mMaterials;
			const int mMAXMATERIALS= 16;
			
			glm::vec3 mTranslation;
			float mAngleX, mAngleY, mAngleZ;
			glm::vec3 mScale;
	};
}
