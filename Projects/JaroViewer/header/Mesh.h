#include "Component3D.h"
#include "Texture2D.h"

#include <vector>

namespace JaroViewer {
	class Mesh : public JaroViewer::Component3D {
		public:
			Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<JaroViewer::Texture2D> textures);

			void load() override;
	};
}
