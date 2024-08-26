#include "Shader.h"
#include "Material.h"
#include <vector>

namespace JaroViewer {
	class Mesh {
		public:
			Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<Material> &materials);

			void Draw(Shader &shader);	

		private:
			std::vector<Material> mMaterials;
			unsigned int mVaoBuffer;
			unsigned int mNumIndices;
	};
}
