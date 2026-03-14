#pragma once

#include "modifiers/modifier.hpp"

namespace JaroViewer {
	template<typename T>
	class AutoModifier : public Modifier {
	private:
		static bool mRegistered;
	};

	template<typename T>
	bool AutoModifier<T>::mRegistered = [] {
		Modifier::registerModifier(&T::getCode());
		return true;
	}();
} // namespace JaroViewer
