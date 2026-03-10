#include "../headers/objectManager.hpp"
#include "../headers/tools.hpp"

#include <iostream>
#include <variant>

using namespace JaroViewer;

ObjectManager::ObjectManager() : mModels(), mShaderManager() {}

MaterialManager* ObjectManager::getMaterialManager() {
	return &mMaterialManager;
}

template<class... Ts> struct Overloaded : Ts... {
	using Ts::operator()...;
};
void ObjectManager::registerModel(
  const std::string& ident,
  const std::vector<float>& vertices,
  ShaderParams shaderParams,
  uint material
) {
	uint shaderIdent = std::visit(
	  Overloaded{
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

void ObjectManager::renderObjects(const glm::vec3& viewPos) {
	for (auto& model : mModels) {
		ModelState& state = model.second;
		glBindVertexArray(state.vao);
		mShaderManager.activateShader(state.shader);
		Shader* shader = mShaderManager.getShader(state.shader);
		mMaterialManager.loadMaterial(shader, state.material);

		// TODO: Apply real instancing
		for (auto& instance : state.instances) {
			if (!instance.active || !instance.render) continue;
			shader->setMat4("model", instance.model);
			shader->setMat3("normalModel", Tools::getNormalModelMatrix(instance.model));
			shader->setVec3("viewPos", viewPos);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, state.count);
		}
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

	glBindVertexArray(0);

	// Create the model entry
	mModels[ident] = ModelState(vao, vertices.size() / 8, false, shader, material, {});
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
