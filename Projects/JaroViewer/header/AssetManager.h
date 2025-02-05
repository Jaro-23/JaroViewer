#pragma once

#include <memory>
#include "./Asset.h"

namespace JaroViewer {

	class AssetManager{
		public:
			static std::shared_ptr<AssetManager> instance();

			template<typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type* = nullptr>
			std::shared_ptr<T> load(const AssetParameter &params);

		protected:
			AssetManager();

		private:
			static std::shared_ptr<AssetManager> m_self;
			std::map<std::string, std::shared_ptr<Asset>> m_assets;
	};


}
