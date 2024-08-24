#include "../header/Model.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

using namespace JaroViewer;

Model::Model(Shader &shader, const std::string &path) : Component3D{shader} { 
	mVaoBuffer = -1; // Set the vao buffer to value so it passes the check at start of render
	loadModel(path); 
}

void Model::load() {}

void Model::defaultRender(const RenderData &data) {	
}

void Model::loadModel(const std::string &path) {
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP" << import.GetErrorString() <<std::endl;
		return;
	}
	mDirectory = path.substr(0, path.find_last_of("/"));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		addComponent(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) 
		processNode(node->mChildren[i], scene);
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene) {
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
		std::vector<Texture2D> diffuseMaps = loadMaterials(material, aiTextureType_DIFFUSE, "texture_diffuse");
		std::vector<Texture2D> specularMaps = loadMaterials(material, aiTextureType_SPECULAR, "texture_specular");
		assert(diffuseMaps.size() == specularMaps.size());
		for (unsigned int i = 0; i < diffuseMaps.size(); i++)
			materials.push_back(Material(diffuseMaps.at(i), specularMaps.at(i), 32.0f));
	}

	return std::shared_ptr<Mesh>( new Mesh(mShader, vertices, indices, materials) );
}

std::vector<Texture2D> Model::loadMaterials(aiMaterial *mat, aiTextureType type, const std::string &typeName) {
	std::vector<Texture2D> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		if (mLoadedTextures.find(str.C_Str()) != mLoadedTextures.end()) {
			textures.push_back(mLoadedTextures.at(str.C_Str()));
			continue;
		} 
		Texture2D texture{mDirectory + "/" + str.C_Str()};
		mLoadedTextures.insert({str.C_Str(), texture});
		textures.push_back(texture);
	}
	return textures;
}
