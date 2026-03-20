#pragma once

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace JaroViewer {
	using ModifierParams = std::vector<float>;

	class Modifier {
	public:
		static std::string getVertexLibrary();
		static std::optional<uint> registerModifier(const std::string& name, const std::string& funcCode);

		void subscribeUpdate(const std::function<void()>& callback);

		virtual ModifierParams getParams() const = 0;

	protected:
		void sendUpdateEvent();

	private:
		struct RegisterEntry {
			uint ident;
			std::string code;
		};

		static std::map<std::string, RegisterEntry>& getModifiers();
		static uint& getNextIdent();

		std::vector<std::function<void()>> mUpdateCallbacks;
		static std::map<std::string, RegisterEntry> mModifiers;
		static uint mNextIdent;
	};
} // namespace JaroViewer
