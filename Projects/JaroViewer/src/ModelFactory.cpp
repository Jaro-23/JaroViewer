#include "../header/ModelFactory.h"
#include "../header/Tools.h"
#include <iostream>
#include <memory>

using namespace JaroViewer;

std::shared_ptr<ModelFactory> ModelFactory::mInstance{nullptr};

std::shared_ptr<ModelFactory> ModelFactory::getInstance() {
	if (mInstance == nullptr)
		mInstance = std::shared_ptr<ModelFactory>( new ModelFactory() );
	return mInstance;
}

void ModelFactory::drawModel(const std::string &path, const glm::mat4 &model, const Component3D::RenderData &data, Shader &shader) {
	if (mLoadedModels.find(path) == mLoadedModels.end())
		createModel(path);

	ModelData &modelData = mLoadedModels.at(path);
	for (unsigned int i = 0; i < modelData.meshes.size(); i++) {
		shader.use();
		shader.setMat4("model", model);
		shader.setMat3("normalModel", Tools::getNormalModelMatrix(model));
		shader.setVec3("viewPos", data.viewPos);
		modelData.meshes.at(i).Draw(shader);
	}
}

ModelFactory::ModelFactory() :
	mLoadedModels{},
	mLoadedTextures{}
{}

void ModelFactory::createModel(const std::string &path) {
	const aiScene *scene = mImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP" << mImporter.GetErrorString() <<std::endl;
		return;
	}
	ModelData data;
	data.directory = path.substr(0, path.find_last_of("/"));
	processNode(scene->mRootNode, data, scene);
	mLoadedModels.insert({path, data});
}

void ModelFactory::processNode(aiNode *node, ModelData &data, const aiScene *scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		data.meshes.push_back(processMesh(mesh, data.directory, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) 
		processNode(node->mChildren[i], data, scene);
}

Mesh ModelFactory::processMesh(aiMesh *mesh, const std::string directory, const aiScene *scene) {
	std::vector<float> vertices{};
	std::vector<unsigned int> indices{};
	std::vector<Material> materials{};

	// Process the vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
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
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture2D> diffuseMaps = loadMaterials(material, aiTextureType_DIFFUSE, directory, "texture_diffuse");
		std::vector<Texture2D> specularMaps = loadMaterials(material, aiTextureType_SPECULAR, directory, "texture_specular");
		assert(diffuseMaps.size() == specularMaps.size());
		for (unsigned int i = 0; i < diffuseMaps.size(); i++)
			materials.push_back(Material(diffuseMaps.at(i), specularMaps.at(i), 32.0f));
	}

	return Mesh(vertices, indices, materials);
}

std::vector<Texture2D> ModelFactory::loadMaterials(aiMaterial *mat, aiTextureType type, const std::string directory, const std::string &typeName) {
	std::vector<Texture2D> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		if (mLoadedTextures.find(str.C_Str()) != mLoadedTextures.end()) {
			textures.push_back(mLoadedTextures.at(str.C_Str()));
			continue;
		} 
		Texture2D texture{directory + "/" + str.C_Str()};
		mLoadedTextures.insert({str.C_Str(), texture});
		textures.push_back(texture);
	}
	return textures;
}
