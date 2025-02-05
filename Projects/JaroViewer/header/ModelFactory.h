#pragma once

#include "Component3D.h"
#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include <memory>

namespace JaroViewer {
	class ModelFactory {
		public:
			static std::shared_ptr<ModelFactory> getInstance();

			void drawModel(const std::string &path, const glm::mat4 &model, const Component3D::RenderData &data, std::shared_ptr<Shader> &shader);

		private:
			static std::shared_ptr<ModelFactory> mInstance;

			struct ModelData {
				std::vector<JaroViewer::Mesh> meshes;
				std::string directory;	
			};
			
			Assimp::Importer mImporter;
			std::map<std::string, ModelData> mLoadedModels;
			std::map<std::string, JaroViewer::Texture2D> mLoadedTextures;

			ModelFactory();

			void createModel(const std::string &path);
			void processNode(aiNode *node, ModelData &data, const aiScene *scene);
			Mesh processMesh(aiMesh *mesh, const std::string directory, const aiScene *scene);
			std::vector<Texture2D> loadMaterials(aiMaterial *mat, aiTextureType type, const std::string directory, const std::string &typeName);
	};
}
