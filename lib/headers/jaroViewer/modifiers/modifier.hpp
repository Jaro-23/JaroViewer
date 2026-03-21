#pragma once

#include "jaroViewer/core/eventSender.hpp"
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace JaroViewer {
	using ModifierParams = std::vector<float>;

	enum ModifierEvent { UPDATE };

	class Modifier : public EventSender<Modifier, ModifierEvent> {
	public:
		static std::string getVertexLibrary();
		static std::optional<uint> registerModifier(const std::string& name, const std::string& funcCode);

		virtual ModifierParams getParams() const = 0;

	private:
		struct RegisterEntry {
			uint ident;
			std::string code;
		};

		static std::map<std::string, RegisterEntry>& getModifiers();
		static uint& getNextIdent();

		static std::map<std::string, RegisterEntry> mModifiers;
		static uint mNextIdent;
	};
} // namespace JaroViewer
