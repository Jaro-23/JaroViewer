#pragma once

#include "autoModifier.hpp"

namespace JaroViewer {
	class WavingModifier : public AutoModifier<WavingModifier> {
	public:
		static std::string getCode();
		ModifierParams getParams() const override;
	};
} // namespace JaroViewer
