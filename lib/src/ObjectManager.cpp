#include "../headers/ObjectManager.hpp"

#include "Tools.hpp"
#include <iostream>

using namespace JaroViewer;

ObjectManager::ObjectManager() : mModels() {}

void ObjectManager::registerModel(const std::string& ident, const std::vector<float>& vertices) {
	registerFullModel(ident, vertices, 0);
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

void ObjectManager::registerFullModel(const std::string& ident, const std::vector<float>& vertices, uint shader) {
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
	// TODO: Add Material
	mModels[ident] = ModelState(vao, vertices.size() / 8, false, shader, 0, {});
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
