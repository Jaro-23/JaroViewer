#pragma once

#include <map>
#include <string>
#include <vector>

namespace JaroViewer {
	#define AssetParameter std::map<std::string, std::vector<std::string>>

	class Asset{
		public:
			Asset() { m_ready = false; };
			virtual bool load(const AssetParameter &params);
			bool isReady() const { return m_ready; };

			static std::string getKey(const AssetParameter &params);

		protected:
			bool m_ready;

		private:
			static std::string processAssetParams(const AssetParameter &params);
	};
}
