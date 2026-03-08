#pragma once

#include "Object.hpp"

#include <map>
#include <string>
#include <sys/types.h>
#include <vector>

namespace JaroViewer {
	struct Instance {
		bool active;
		bool render;
		glm::mat4 model;
	};

	struct ModelState {
		uint vao;
		uint numLines;
		bool useIndices;
		uint shader;
		uint material;
		std::vector<Instance> instances;
	};

	class ObjectManager {
	public:
		ObjectManager();

		void registerModel(const std::string& ident, const std::vector<float>& vertices);
		Object createObject(const std::string& model);

	private:
		void registerFullModel(const std::string& ident, const std::vector<float>& vertices, uint shader);
		size_t getNextFreeSlot(const std::string& model) const;

		std::map<std::string, ModelState> mModels;
	};
} // namespace JaroViewer
