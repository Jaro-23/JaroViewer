#pragma once

#include "glm/fwd.hpp"
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace JaroViewer {
	class Modifier {
	public:
		static std::string getVertexLibrary();
		static std::optional<uint> registerModifier(const std::string& name, const std::string& funcCode);

		virtual std::vector<glm::vec4> getParams() const = 0;

	private:
		struct RegisterEntry {
			uint ident;
			std::string code;
		};

		static std::map<std::string, RegisterEntry> mModifiers;
		static uint mNextIdent;
	};
} // namespace JaroViewer
