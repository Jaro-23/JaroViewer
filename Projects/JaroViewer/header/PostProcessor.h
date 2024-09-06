#pragma once

#include "FrameBuffer.h"
#include "Shader.h"

#include <memory>
#include <string>

namespace JaroViewer {
	class PostProcessor {
		public:
			PostProcessor(int width, int height, const std::string fragmentFile);
			~PostProcessor() { mFrameBuffer.deleteBuffer(); };
			void setupVao();

			void bind() const;
			void unbind() const;
			void clear(float r, float g, float b, float a) const;
			void render() const;

		private:
			std::unique_ptr<Shader> mShader;
			FrameBuffer mFrameBuffer;
			unsigned int mVaoBuffer;
	};
}
