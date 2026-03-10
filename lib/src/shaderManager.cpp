#include "../headers/shaderManager.hpp"
#include "../headers/basicShaders.hpp"
#include <sstream>

using namespace JaroViewer;

ShaderManager::ShaderManager() : mPathToIdent(), mShaders(), mActiveShader(0) {
	loadShader(ShaderCode{basicWhiteVertex, "", basicWhiteFragment});
	mShaders.at(0).setUniformBuffer("Transformation", 0);
	mShaders.at(0).use();
}

uint ShaderManager::loadShader(const ShaderCode& codes) {
	mShaders.push_back(Shader(codes));
	uint ident = mShaders.size() - 1;
	return ident;
}

uint ShaderManager::loadShader(const ShaderPaths& paths) {
	std::string key = pathsToKey(paths);
	if (mPathToIdent.contains(key)) return mPathToIdent.at(key);
	mShaders.push_back(Shader(paths));

	uint ident        = mShaders.size() - 1;
	mPathToIdent[key] = ident;
	return ident;
}

Shader* ShaderManager::getShader(uint ident) {
	if (ident >= mShaders.size()) return nullptr;
	return &mShaders.at(ident);
}

void ShaderManager::activateShader(uint ident) {
	if (mActiveShader == ident) return;
	mShaders.at(ident).use();
	mActiveShader = ident;
}

std::string ShaderManager::pathsToKey(const ShaderPaths& paths) const {
	std::stringstream output;
	for (auto& path : paths.vertexPaths) output << "|" << path;
	for (auto& path : paths.geometryPaths) output << "|" << path;
	for (auto& path : paths.fragmentPaths) output << "|" << path;
	return output.str();
}
