#include "Component3D.h"

#include <vector>

namespace JaroViewer {
	class Mesh : public JaroViewer::Component3D {
		public:
			Mesh(const Shader &shader, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<Material> &materials);

			void load() override;
	};
}
