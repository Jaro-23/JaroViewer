#include "rendering/shaderManager.hpp"
#include "modifiers/modifier.hpp"
#include "rendering/basicShaders.hpp"

#include <iostream>
#include <sstream>

using namespace JaroViewer;

ShaderManager::ShaderManager() : mPathToIdent(), mShaders(), mActiveShader(0) {
	loadShader(ShaderCode{basicWhiteVertex, "", basicWhiteFragment});
	loadShader(ShaderCode{Modifier::getVertexLibrary() + basicVertex, "", fragmentLibrary + basicFragment});
	mShaders.at(0).use();
}

uint ShaderManager::loadShader(const ShaderCode& codes) {
	mShaders.push_back(Shader(codes));
	mShaders.back().setUniformBuffer("Transformation", 0);
	mShaders.back().setUniformBuffer("LightSet", 1);
	return mShaders.size() - 1;
}

uint ShaderManager::loadShader(const ShaderPaths& paths) {
	std::string key = pathsToKey(paths);
	if (mPathToIdent.contains(key)) return mPathToIdent.at(key);
	mShaders.push_back(Shader(paths));
	mShaders.back().setUniformBuffer("Transformation", 0);
	mShaders.back().setUniformBuffer("LightSet", 1);

	uint ident        = mShaders.size() - 1;
	mPathToIdent[key] = ident;
	return ident;
}

Shader* ShaderManager::getShader(uint ident) {
	if (ident >= mShaders.size()) return nullptr;
	return &mShaders.at(ident);
}

bool ShaderManager::activateShader(uint ident) {
	if (mActiveShader == ident) return false;
	mShaders.at(ident).use();
	mActiveShader = ident;
	return true;
}

std::string ShaderManager::pathsToKey(const ShaderPaths& paths) const {
	std::stringstream output;
	for (auto& path : paths.vertexPaths) output << "|" << path;
	for (auto& path : paths.geometryPaths) output << "|" << path;
	for (auto& path : paths.fragmentPaths) output << "|" << path;
	return output.str();
}
