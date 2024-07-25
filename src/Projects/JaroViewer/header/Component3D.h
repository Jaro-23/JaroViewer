#pragma once

#include "Shader.h"
#include <GLM/glm.hpp>

namespace JaroViewer {
	class Component3D {
		public:
			// Defining basic communications strcutures for the component		
			typedef struct RenderData {
				glm::mat4 model;
				glm::mat4 view;
				glm::mat4 projection;
			} RenderData;

			Component3D(const Shader &shader);
			void setWireframeMode(bool enable);

			virtual void load() = 0;
			void render(const RenderData &data);

		protected:
			bool getWireframeMode() const;

			template <typename T, typename Allocator>
			unsigned int generateBuffer(std::vector<T, Allocator> &data, GLenum bufferType, GLenum drawType) {
				unsigned int buffer;
				glGenBuffers(1, &buffer);
				glBindBuffer(bufferType, buffer);
				glBufferData(bufferType, data.size() * sizeof(T), &data[0], drawType);
				return buffer;
			}

			unsigned int mNumLines = 36;
			unsigned int mVaoBuffer = 0;
			Shader mShader;

		private:
			virtual void defaultRender(const RenderData &data);

			Shader mWireframeShader;
			bool mWireframeMode;
	};
}
