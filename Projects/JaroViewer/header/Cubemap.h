#pragma once

#include "Shader.h"
#include <string>
#include <vector>

namespace JaroViewer {
	class Cubemap {
		public:
			Cubemap(const std::vector<std::string> &paths);
			Cubemap(const std::string &dir);
			void render() const;
			
			void bind() const;
			void unbind() const;

		private:
			void loadVao();
			void getPaths(const std::string dir, std::vector<std::string>* out) const;
			void loadMap(const std::vector<std::string>& paths);
			void genBuffer();
			void bindTextures(const std::vector<std::string>& paths);
			void setupParams() const;

			unsigned int mCubemap;
			unsigned int mVaoBuffer;
			Shader mShader;
	};
}
