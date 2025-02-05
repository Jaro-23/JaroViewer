#include "../header/Asset.h"
#include <iostream>

using namespace JaroViewer;

bool Asset::load(const AssetParameter &params) { return false; }

std::string Asset::getKey(const AssetParameter &params) { 
	std::string key = "ASSET" + Asset::processAssetParams(params);
	std::cout << "WARNING:ASSET:DEFAULT_KEY_USED:" << key << std::endl;
	return key;
}

std::string Asset::processAssetParams(const AssetParameter &params) {
	std::string result = "?";
	for (const auto& [key, value] : params) {
		if (result.length() > 1) result += ";";
		result += key + "=[";
		for (int i = 0; i < value.size(); i++) {
			if (i > 0) result += ",";
			result += value.at(i);
		}
		result += "]";
	}
	return result;
}
