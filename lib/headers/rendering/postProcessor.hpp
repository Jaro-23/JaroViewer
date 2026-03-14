#pragma once

#include "core/window.hpp"
#include "graphics/frameBuffer.hpp"
#include "rendering/shader.hpp"

#include <memory>
#include <string>

namespace JaroViewer {
	class PostProcessor {
	public:
		PostProcessor(Window* window, const std::string fragmentFile);
		void setupVao();

		void bindAndClear(float r, float g, float b, float a) const;
		void unbind() const;
		void render() const;

	private:
		std::unique_ptr<Shader> mShader;
		FrameBuffer mFrameBuffer;
		unsigned int mVaoBuffer;
	};
} // namespace JaroViewer
