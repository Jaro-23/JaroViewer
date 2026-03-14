#pragma once

#include "graphics/materialManager.hpp"
#include "rendering/shader.hpp"
#include "rendering/shaderManager.hpp"
#include "scene/object.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <map>
#include <string>
#include <sys/types.h>
#include <variant>
#include <vector>

namespace JaroViewer {
	struct Instance {
		bool active;
		bool render;
		glm::mat4 model;
	};

	struct Mesh {
		uint vao;
		uint instanceVBO;
		uint count;
		uint material;
	};

	struct ModelState {
		std::vector<Mesh> meshes;
		bool useIndices;
		uint shader;
		std::vector<Instance> instances;
	};

	using ShaderParams =
	  std::variant<std::reference_wrapper<const ShaderCode>, std::reference_wrapper<const ShaderPaths>, uint>;

	class ObjectManager {
	public:
		ObjectManager();

		MaterialManager* getMaterialManager();

		void registerModel(const std::string& ident, const std::vector<float>& vertices, ShaderParams shaderParams, uint material);
		void registerModel(const std::string& ident, const std::string& modelPath, ShaderParams shaderParams);
		Object createObject(const std::string& model);

		void renderObjects(bool usingPostProcessor, const glm::vec3& viewPos);

	private:
		Mesh registerVerticesModel(const std::vector<float>& vertices, uint material);
		Mesh registerIndicesModel(const std::vector<float>& vertices, const std::vector<uint>& indices, uint material);
		uint handleBuffers();

		void registerFileModel(const std::string& ident, const std::string& modelPath, uint shader);
		void processNode(aiNode* node, const std::string& ident, const std::string& directory, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const std::string& directory, const aiScene* scene);
		std::vector<std::string> loadMaterials(aiMaterial* mat, aiTextureType type);

		size_t getNextFreeSlot(const std::string& model) const;

		std::map<std::string, ModelState> mModels;
		ShaderManager mShaderManager;
		MaterialManager mMaterialManager;
		std::shared_ptr<Assimp::Importer> mImporter;
	};
} // namespace JaroViewer
