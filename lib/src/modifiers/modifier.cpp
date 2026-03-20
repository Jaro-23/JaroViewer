#include "modifiers/modifier.hpp"
#include "rendering/basicShaders.hpp"

#include <iostream>
#include <optional>
#include <sstream>

using namespace JaroViewer;

std::string Modifier::getVertexLibrary() {
	std::stringstream out;
	out << vertexLibrary;
	out << "uint modifierIndex = 0;\n"
	       "float nextModifierParam() {\n"
	       "float ret = getDataFromVector(modifierData, modifierIndex);\n"
	       "modifierIndex = modifierIndex + 1;\n"
	       "return ret;\n"
	       "}\n";

	for (auto& modifier : getModifiers())
		out << "vec3 " << modifier.first << "(vec3 inVec) {\n"
		    << modifier.second.code << "\n}\n";

	out << "vec3 processModifiers(vec3 inVec) {\n"
	       "modifierIndex = aModifierStart;\n"
	       "vec3 outVec = inVec;\n"
	       "for (int i = 0; i < aModifierCount; ++i) {\n"
	       "int type = int(nextModifierParam());\n"
	       "switch (type) {";
	for (auto& modifier : getModifiers())
		out << "case " << modifier.second.ident << ": outVec = " << modifier.first
		    << "(outVec); break;\n";
	out << "}\n}\nreturn outVec;\n}\n";

	return out.str();
}

std::optional<uint> Modifier::registerModifier(const std::string& name, const std::string& funcCode) {
	auto& mods      = getModifiers();
	auto& nextIdent = getNextIdent();

	if (mods.contains(name)) {
		std::cerr
		  << "[Modifier] Error: Trying to register already registered modifier \'"
		  << name << "\'" << std::endl;
		return std::nullopt;
	}
	mods[name] = RegisterEntry(nextIdent, funcCode);
	return nextIdent++;
}

void Modifier::subscribeUpdate(const std::function<void()>& callback) {
	mUpdateCallbacks.push_back(callback);
}

void Modifier::sendUpdateEvent() {
	for (auto& callback : mUpdateCallbacks) callback();
}

std::map<std::string, Modifier::RegisterEntry>& Modifier::getModifiers() {
	static std::map<std::string, RegisterEntry> instance;
	return instance;
}
uint& Modifier::getNextIdent() {
	static uint instance = 0;
	return instance;
}
