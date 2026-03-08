#pragma once

#include "Object.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"

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

		void registerModel(const std::string& ident, const std::vector<float>& vertices, ShaderParams shaderParams);
		Object createObject(const std::string& model);

		void renderObjects();

	private:
		void registerFullModel(const std::string& ident, const std::vector<float>& vertices, uint shader);
		size_t getNextFreeSlot(const std::string& model) const;

		std::map<std::string, ModelState> mModels;
		ShaderManager mShaderManager;
	};
} // namespace JaroViewer
