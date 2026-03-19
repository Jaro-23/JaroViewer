#include "modifiers/modifier.hpp"
#include "rendering/basicShaders.hpp"

#include <iostream>
#include <optional>
#include <sstream>

using namespace JaroViewer;

uint Modifier::mNextIdent = 0;
std::map<std::string, Modifier::RegisterEntry> JaroViewer::Modifier::mModifiers = {};

std::string Modifier::getVertexLibrary() {
	std::stringstream out;
	out << vertexLibrary;
	out << "int modifierIndex = 0;\n"
	       "float nextModifierParam(uint index) {\n"
	       "float ret = getDataFromVector(modifierData, modifierIndex);\n"
	       "modifierIndex = modifierIndex + 1;\n"
	       "return ret;\n"
	       "}\n";

	for (auto& modifier : mModifiers)
		out << "void " << modifier.first << "() {\n"
		    << modifier.second.code << "\n}\n";

	out << "void processModifiers(uint base) {\n"
	       "for (int i = 0; i < aModifierCount; ++i) {\n"
	       "int type = int(nextModifierParam());\n"
	       "switch (type) {";
	for (auto& modifier : mModifiers)
		out << "case " << modifier.first << ": " << modifier.second.ident << "(); break;\n";
	out << "}\n}\n}\n";

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

void Modifier::subscribeUpdate(const std::function<void()>& callback) {
	mUpdateCallbacks.push_back(callback);
}

void Modifier::sendUpdateEvent() {
	for (auto& callback : mUpdateCallbacks) {
		callback();
	}
}
