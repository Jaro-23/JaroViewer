#include "../headers/ShaderManager.hpp"
#include <sstream>

using namespace JaroViewer;

ShaderManager::ShaderManager() : mPathToIdent(), mShaders(), mActiveShader(0) {}

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
