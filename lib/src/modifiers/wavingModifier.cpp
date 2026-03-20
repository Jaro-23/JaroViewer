#include "modifiers/wavingModifier.hpp"
#include <cassert>

using namespace JaroViewer;

std::string WavingModifier::getCode() {
	return "vec3 outVec = inVec;\n outVec.y += "
	       "sin(inVec.x) * 0.2;\n return outVec;\n";
}

ModifierParams WavingModifier::getParams() const {
	assert(WavingModifier::mRegistered);
	return {(float)WavingModifier::mIdent};
}
