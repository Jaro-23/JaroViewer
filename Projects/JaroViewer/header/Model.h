#pragma once

#include "Material.h"
#include "Mesh.h"
#include "Texture2D.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include <memory>

namespace JaroViewer {
	class Model : public JaroViewer::Component3D {
		public:
			Model(Shader &shader, const std::string &path);
			void load() override;
		
		private:
			std::vector<JaroViewer::Mesh> mMeshes;
			std::map<std::string, JaroViewer::Texture2D> mLoadedTextures;
			std::string mDirectory;
			
			void defaultRender(const RenderData &data) override;
			void loadModel(const std::string &path);
			void processNode(aiNode *node, const aiScene *scene);
			std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
			std::vector<Texture2D> loadMaterials(aiMaterial *mat, aiTextureType type, const std::string &typeName);
	};
}
