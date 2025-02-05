#include "../header/AssetManager.h"
#include "../header/Shader.h"
#include <memory>

using namespace JaroViewer;

// Definitions of the template function
template std::shared_ptr<Asset> AssetManager::load<Asset>(const AssetParameter &params);
template std::shared_ptr<Shader> AssetManager::load<Shader>(const AssetParameter &params);
	
std::shared_ptr<AssetManager> AssetManager::m_self{nullptr};

std::shared_ptr<AssetManager> AssetManager::instance() {
	if (m_self == nullptr) {
		AssetManager manager{};
		m_self = std::make_shared<AssetManager>(manager);
	}
	return m_self;
}

template<typename T, typename std::enable_if<std::is_base_of<Asset, T>::value>::type*>
std::shared_ptr<T> AssetManager::load(const AssetParameter &params) {
	std::string key = T::getKey(params);
	if (m_assets.contains(key)) // Clangd gives error but exists since C++20
		return std::dynamic_pointer_cast<T>(m_assets.at(key));

	m_assets[key] = std::make_shared<T>();
	m_assets[key]->load(params);
	return std::dynamic_pointer_cast<T>(m_assets.at(key));
}

AssetManager::AssetManager() {}
