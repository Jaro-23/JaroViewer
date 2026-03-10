#pragma once

#include "materialManager.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "shaderManager.hpp"

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

	struct ModelState {
		uint vao;
		uint count;
		bool useIndices;
		uint shader;
		uint material;
		std::vector<Instance> instances;
	};

	using ShaderParams =
	  std::variant<std::reference_wrapper<const ShaderCode>, std::reference_wrapper<const ShaderPaths>, uint>;

	class ObjectManager {
	public:
		ObjectManager();

		MaterialManager* getMaterialManager();

		void registerModel(const std::string& ident, const std::vector<float>& vertices, ShaderParams shaderParams, uint material);
		Object createObject(const std::string& model);

		void renderObjects(const glm::vec3& viewPos);

	private:
		void registerFullModel(const std::string& ident, const std::vector<float>& vertices, uint shader, uint material);
		size_t getNextFreeSlot(const std::string& model) const;

		std::map<std::string, ModelState> mModels;
		ShaderManager mShaderManager;
		MaterialManager mMaterialManager;
	};
} // namespace JaroViewer
