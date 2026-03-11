#include "../headers/objectManager.hpp"
#include "../headers/tools.hpp"

#include <iostream>
#include <variant>

using namespace JaroViewer;

ObjectManager::ObjectManager() : mModels(), mShaderManager() {}

MaterialManager* ObjectManager::getMaterialManager() {
	return &mMaterialManager;
}

void ObjectManager::registerModel(
  const std::string& ident,
  const std::vector<float>& vertices,
  ShaderParams shaderParams,
  uint material
) {
	uint shaderIdent = std::visit(
	  Tools::Overloaded{
	    [&](const ShaderCode& codes) { return mShaderManager.loadShader(codes); },
	    [&](const ShaderPaths& paths) { return mShaderManager.loadShader(paths); },
	    [&](uint id) { return id; }
	  },
	  shaderParams
	);
	registerFullModel(ident, vertices, shaderIdent, material);
}

Object ObjectManager::createObject(const std::string& model) {
	Object obj{};

	// Check if valid model and find the index to work with
	if (!mModels.contains(model)) {
		std::cerr
		  << "[Object Manager] Error: Tried to create object with unknown model \'"
		  << model << "\'" << std::endl;
		return obj;
	}
	size_t index = getNextFreeSlot(model);

	// Create the instance
	if (index == mModels.at(model).instances.size()) {
		mModels.at(model).instances.push_back(Instance{true, true, obj.getModelMatrix()});
	} else {
		Instance& instance = mModels.at(model).instances.at(index);
		instance.active    = true;
		instance.render    = true;
		instance.model     = obj.getModelMatrix();
	}

	// Link all events
	obj.subscribeDelete([this, model, index]() {
		this->mModels.at(model).instances.at(index).active = false;
	});
	obj.subscribeTransform([this, model, index](glm::mat4 mat) {
		this->mModels.at(model).instances.at(index).model = mat;
	});
	obj.subscribeVisibility([this, model, index](bool visibility) {
		this->mModels.at(model).instances.at(index).render = visibility;
	});

	return obj;
}

void ObjectManager::renderObjects(bool usingPostProcessor, const glm::vec3& viewPos) {
	if (usingPostProcessor) mMaterialManager.resetLastShader();
	for (auto& model : mModels) {
		ModelState& state = model.second;
		glBindVertexArray(state.vao);
		mShaderManager.activateShader(state.shader);
		Shader* shader = mShaderManager.getShader(state.shader);
		mMaterialManager.loadMaterial(shader, state.material);

		struct InstanceData {
			glm::mat4 model;
			glm::mat3 normalModel;
		};

		std::vector<InstanceData> data;
		for (auto& instance : state.instances) {
			if (!instance.active || !instance.render) continue;
			data.push_back({instance.model, Tools::getNormalModelMatrix(instance.model)});
		}

		glBindBuffer(GL_ARRAY_BUFFER, state.instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(InstanceData), data.data(), GL_DYNAMIC_DRAW);

		shader->setVec3("viewPos", viewPos);
		glDrawArraysInstanced(GL_TRIANGLES, 0, state.count, data.size());
	}
}

void ObjectManager::registerFullModel(const std::string& ident, const std::vector<float>& vertices, uint shader, uint material) {
	if (mModels.contains(ident)) {
		std::cerr << "[Object Manager] Error: Already a model with iden \'" + ident + "\'"
		          << std::endl;
		return;
	}

	// Create the vao
	uint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	Tools::generateBuffer(vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	uint instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

	// mat4 model at locations 3-6
	for (int i = 0; i < 4; i++) {
		glVertexAttribPointer(
		  3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(glm::mat3),
		  (void*)(i * sizeof(glm::vec4))
		);
		glEnableVertexAttribArray(3 + i);
		glVertexAttribDivisor(3 + i, 1); // ← advance once per instance
	}
	// mat3 normalModel at locations 7-9
	for (int i = 0; i < 3; i++) {
		glVertexAttribPointer(
		  7 + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(glm::mat3),
		  (void*)(sizeof(glm::mat4) + i * sizeof(glm::vec3))
		);
		glEnableVertexAttribArray(7 + i);
		glVertexAttribDivisor(7 + i, 1);
	}

	glBindVertexArray(0);

	// Create the model entry
	mModels[ident] =
	  ModelState(vao, instanceVBO, vertices.size() / 8, false, shader, material, {});
}

size_t ObjectManager::getNextFreeSlot(const std::string& model) const {
	const std::vector<Instance>& instances = mModels.at(model).instances;
	for (size_t i = 0; i < instances.size(); ++i) {
		if (!instances.at(i).active) {
			return i;
		}
	}
	return instances.size();
}
