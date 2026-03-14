#include "modifiers/modifier.hpp"
#include "rendering/basicShaders.hpp"

#include <iostream>
#include <optional>
#include <sstream>

using namespace JaroViewer;

uint Modifier::mNextIdent = 0;

std::string Modifier::getVertexLibrary() {
	std::stringstream out;
	out << vertexLibrary;

	for (auto& modifier : mModifiers)
		out << "void " << modifier.first << "() {\n"
		    << modifier.second.code << "\n}\n";

	return out.str();
}

std::optional<uint> Modifier::registerModifier(const std::string& name, const std::string& funcCode) {
	if (mModifiers.contains(name)) {
		std::cerr
		  << "[Modifier] Error: Trying to register already registered modifier \'"
		  << name << "\'" << std::endl;
		return std::nullopt;
	}
	mModifiers[name] = RegisterEntry(mNextIdent, funcCode);
	return mNextIdent++;
}
