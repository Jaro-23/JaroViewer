#include "Shader.h"
#include "Material.h"
#include <memory>
#include <vector>

namespace JaroViewer {
	class Mesh {
		public:
			Mesh(const std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<Material> &materials);
			void Draw(const std::shared_ptr<Shader> &shader);	

		private:
			std::vector<Material> mMaterials;
			unsigned int mVaoBuffer;
			unsigned int mNumIndices;
	};
}
