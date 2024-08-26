#pragma once

#include "Component3D.h"

namespace JaroViewer {
	class Model : public JaroViewer::Component3D {
		public:
			Model(Shader &shader, const std::string &path);
			void load() override;
		
		private:
			void wireframeRender(const RenderData &data) override;
			void defaultRender(const RenderData &data) override;

			std::string mPath;
	};
}
