#include "Component3D.h"

namespace JaroViewer {
	class BoxComponent : public Component3D {
		public:
			BoxComponent(const Shader &shader, const Shader &wireframeShader);

			void load() override;
		private:
	};
}
