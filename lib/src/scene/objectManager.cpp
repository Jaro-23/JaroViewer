#include "jaroViewer/scene/objectManager.hpp"
#include "jaroViewer/core/tools.hpp"
#include "jaroViewer/rendering/gpuVector.hpp"
#include "jaroViewer/scene/object.hpp"

#include <cstddef>
#include <iostream>
#include <limits>
#include <variant>

using namespace JaroViewer;

ObjectManager::ObjectManager() : mModels(), mShaderManager() {
	mImporter = std::make_shared<Assimp::Importer>();
}

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
	if (mModels.contains(ident)) {
		std::cerr << "[Object Manager] Error: Already a model with iden \'" + ident + "\'"
		          << std::endl;
		return;
	}
	Mesh mesh = registerVerticesModel(vertices, material);
	mModels[ident] =
	  ModelState(std::vector<Mesh>{mesh}, false, shaderIdent, GpuVector(), {});
}

void ObjectManager::registerModel(const std::string& ident, const std::string& modelPath, ShaderParams shaderParams) {
	uint shaderIdent = std::visit(
	  Tools::Overloaded{
	    [&](const ShaderCode& codes) { return mShaderManager.loadShader(codes); },
	    [&](const ShaderPaths& paths) { return mShaderManager.loadShader(paths); },
	    [&](uint id) { return id; }
	  },
	  shaderParams
	);
	registerFileModel(ident, modelPath, shaderIdent);
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
		mModels.at(model).instances.push_back(Instance{true, true, obj.getModelMatrix(), 0, 0});
	} else {
		Instance& instance     = mModels.at(model).instances.at(index);
		instance.active        = true;
		instance.render        = true;
		instance.model         = obj.getModelMatrix();
		instance.modifierStart = 0;
		instance.modifierCount = 0;
	}

	// Link all events
	obj.addListener([this, model, index](Object* obj, ObjectEvent event) {
		switch (event) {
		case ObjectEvent::MODIFIER:
			this->updateModifierTex(obj->getStack(), model, index);
			break;
		case ObjectEvent::DELETE:
			this->mModels.at(model).instances.at(index).active = false;
			break;
		case ObjectEvent::TRANSFORM:
			this->mModels.at(model).instances.at(index).model = obj->getModelMatrix();
			break;
		case ObjectEvent::VISIBILITY:
			this->mModels.at(model).instances.at(index).render = obj->getVisibility();
			break;
		}
	});

	return obj;
}

void ObjectManager::renderObjects(bool usingPostProcessor, const glm::vec3& viewPos) {
	if (usingPostProcessor) mMaterialManager.resetLastShader();
	for (auto& model : mModels) {
		ModelState& state = model.second;

		std::vector<InstanceData> data;
		for (auto& instance : state.instances) {
			if (!instance.active || !instance.render) continue;
			data.push_back(
			  {instance.model, Tools::getNormalModelMatrix(instance.model),
			   instance.modifierStart, instance.modifierCount}
			);
		}

		for (Mesh& mesh : state.meshes) {
			glBindVertexArray(mesh.vao);

			mShaderManager.activateShader(state.shader);
			Shader* shader = mShaderManager.getShader(state.shader);
			shader->setInt("modifierData", 0);
			state.modifierData.load(0);
			mMaterialManager.loadMaterial(shader, mesh.material, 1);

			glBindBuffer(GL_ARRAY_BUFFER, mesh.instanceVBO);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(InstanceData), data.data(), GL_DYNAMIC_DRAW);

			shader->setVec3("minPoint", mesh.minPoint);
			shader->setVec3("maxPoint", mesh.maxPoint);
			shader->setVec3("viewPos", viewPos);
			if (state.useIndices)
				glDrawElementsInstanced(
				  GL_TRIANGLES, mesh.count, GL_UNSIGNED_INT, 0, data.size()
				);
			else
				glDrawArraysInstanced(GL_TRIANGLES, 0, mesh.count, data.size());
		}
	}
}

void ObjectManager::updateModifierTex(const ModifierStack& stack, const std::string& model, uint instanceIdent) {
	// Update the data for the instance
	ModelState& state = mModels.at(model);
	Instance& ins     = state.instances.at(instanceIdent);

	if (ins.modifierCount == 0) ins.modifierStart = state.modifierData.count();
	ins.modifierCount = stack.count;

	size_t nextStack = 0;
	for (size_t i = instanceIdent + 1; i < state.instances.size(); ++i) {
		Instance& nextIns = state.instances.at(i);
		if (nextIns.modifierCount <= 0) continue;
		nextStack = nextIns.modifierStart;
		break;
	}

	int count = nextStack - ins.modifierStart;
	if (nextStack > 0 && (uint)count != stack.count) {
		int offset = count - stack.count;
		state.modifierData.move(ins.modifierStart, ins.modifierStart + offset);
		for (size_t i = instanceIdent + 1; i < state.instances.size(); ++i) {
			Instance& nextIns = state.instances.at(i);
			if (nextIns.modifierCount <= 0) continue;
			nextIns.modifierStart = nextIns.modifierStart - offset;
		}
	}

	state.modifierData.copy(stack.params, ins.modifierStart);
}

Mesh ObjectManager::registerVerticesModel(const std::vector<float>& vertices, uint material) {
	// Create the vao
	uint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glm::vec3 minPoint{std::numeric_limits<float>().max()};
	glm::vec3 maxPoint{std::numeric_limits<float>().min()};
	for (size_t i = 0; i < vertices.size(); i += 8) {
		minPoint.x = std::min(minPoint.x, vertices.at(i));
		minPoint.y = std::min(minPoint.y, vertices.at(i + 1));
		minPoint.z = std::min(minPoint.z, vertices.at(i + 2));

		maxPoint.x = std::max(maxPoint.x, vertices.at(i));
		maxPoint.y = std::max(maxPoint.y, vertices.at(i + 1));
		maxPoint.z = std::max(maxPoint.z, vertices.at(i + 2));
	}

	Tools::generateBuffer(vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	uint instanceVBO = handleBuffers();

	glBindVertexArray(0);
	return Mesh(vao, instanceVBO, vertices.size() / 8, material, minPoint, maxPoint);
}

Mesh ObjectManager::registerIndicesModel(
  const std::vector<float>& vertices,
  const std::vector<uint>& indices,
  uint material
) {
	uint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glm::vec3 minPoint{std::numeric_limits<float>().max()};
	glm::vec3 maxPoint{std::numeric_limits<float>().min()};
	for (size_t i = 0; i < vertices.size(); i += 8) {
		minPoint.x = std::min(minPoint.x, vertices.at(i));
		minPoint.y = std::min(minPoint.y, vertices.at(i + 1));
		minPoint.z = std::min(minPoint.z, vertices.at(i + 2));

		maxPoint.x = std::max(maxPoint.x, vertices.at(i));
		maxPoint.y = std::max(maxPoint.y, vertices.at(i + 1));
		maxPoint.z = std::max(maxPoint.z, vertices.at(i + 2));
	}

	Tools::generateBuffer(vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	Tools::generateBuffer(indices, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	uint instanceVBO = handleBuffers();

	glBindVertexArray(0);
	return Mesh(vao, instanceVBO, indices.size(), material, minPoint, maxPoint);
}

uint ObjectManager::handleBuffers() {
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
		  3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(i * sizeof(glm::vec4))
		);
		glEnableVertexAttribArray(3 + i);
		glVertexAttribDivisor(3 + i, 1);
	}
	// mat3 aNormalModel at locations 7-9
	for (int i = 0; i < 3; i++) {
		glVertexAttribPointer(
		  7 + i, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
		  (void*)(offsetof(InstanceData, normalModel) + i * sizeof(glm::vec3))
		);
		glEnableVertexAttribArray(7 + i);
		glVertexAttribDivisor(7 + i, 1);
	}
	// modifier start
	glVertexAttribIPointer(10, 1, GL_UNSIGNED_INT, sizeof(InstanceData), (void*)(offsetof(InstanceData, modifierStart)));
	glEnableVertexAttribArray(10);
	glVertexAttribDivisor(10, 1);
	// modifier count
	glVertexAttribIPointer(11, 1, GL_UNSIGNED_INT, sizeof(InstanceData), (void*)(offsetof(InstanceData, modifierCount)));
	glEnableVertexAttribArray(11);
	glVertexAttribDivisor(11, 1);

	return instanceVBO;
}

void ObjectManager::registerFileModel(const std::string& ident, const std::string& modelPath, uint shader) {
	if (mModels.contains(ident)) {
		std::cerr << "[Object Manager] Error: Already a model with ident \'" + ident + "\'"
		          << std::endl;
		return;
	}

	const aiScene* scene =
	  mImporter->ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "[Object Manager] Error: Failed assimp load => "
		          << mImporter->GetErrorString() << std::endl;
		return;
	}

	mModels[ident] = ModelState(std::vector<Mesh>(), false, shader, GpuVector(), {});
	std::string directory = modelPath.substr(0, modelPath.find_last_of("/"));
	processNode(scene->mRootNode, ident, directory, scene);
}

void ObjectManager::processNode(aiNode* node, const std::string& ident, const std::string& directory, const aiScene* scene) {

	for (uint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mModels.at(ident).meshes.push_back(processMesh(mesh, directory, scene));
	}

	for (uint i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], ident, directory, scene);
}

Mesh ObjectManager::processMesh(aiMesh* mesh, const std::string& directory, const aiScene* scene) {
	std::vector<float> vertices{};
	vertices.reserve(mesh->mNumVertices * 8);
	std::vector<uint> indices{};
	uint materialIdent = 0;

	// Process the vertices
	for (uint i = 0; i < mesh->mNumVertices; i++) {
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);
		vertices.push_back(mesh->mNormals[i].x);
		vertices.push_back(mesh->mNormals[i].y);
		vertices.push_back(mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0]) {
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		} else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}

	// Process indices
	for (uint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<std::string> diffuseStr = loadMaterials(material, aiTextureType_DIFFUSE);
	std::vector<std::string> specularStr = loadMaterials(material, aiTextureType_SPECULAR);
	assert(diffuseStr.size() == specularStr.size());

	materialIdent = mMaterialManager.createNew();
	for (uint i = 0; i < diffuseStr.size(); i++)
		mMaterialManager.addMaterial(
		  materialIdent,
		  {directory + "/" + diffuseStr.at(i), directory + "/" + specularStr.at(i), 32.0f}
		);

	return registerIndicesModel(vertices, indices, materialIdent);
}

std::vector<std::string> ObjectManager::loadMaterials(aiMaterial* mat, aiTextureType type) {
	std::vector<std::string> texNames;
	texNames.reserve(mat->GetTextureCount(type));
	for (uint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		texNames.push_back(str.C_Str());
	}
	return texNames;
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
