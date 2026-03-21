#include "jaroViewer/lighting/lightSet.hpp"

#include <iostream>
#include <memory>

using namespace JaroViewer;

/**
 * Creates a lightset
 */
LightSet::LightSet()
  : mDirectionalLights{std::array<std::shared_ptr<DirectionalLight>, mMAXNUMDIRLIGHTS>()},
    mPointLights{std::array<std::shared_ptr<PointLight>, mMAXNUMPOINTLIGHTS>()},
    mSpotlights{std::array<std::shared_ptr<Spotlight>, mMAXNUMSPOTLIGHTS>()},
    mNumDirLights{0},
    mNumPointLights{0},
    mNumSpotlights{0} {}

void LightSet::addDirLight(std::shared_ptr<DirectionalLight> dirLight) {
	if ((uint)mNumDirLights >= mMAXNUMDIRLIGHTS) {
		std::cout << "[Light Set] Warning: Directional light not added because "
		             "array was full!"
		          << std::endl;
		return;
	}
	mDirectionalLights[mNumDirLights++] = dirLight;
}

void LightSet::addPointLight(std::shared_ptr<PointLight> pointLight) {
	if ((uint)mNumPointLights >= mMAXNUMPOINTLIGHTS) {
		std::cout << "[Light Set] Warning: Pointlight light not added because "
		             "array was full!"
		          << std::endl;
		return;
	}
	mPointLights[mNumPointLights++] = pointLight;
}

void LightSet::addSpotlight(std::shared_ptr<Spotlight> spotlight) {
	if ((uint)mNumSpotlights >= mMAXNUMSPOTLIGHTS) {
		std::cout << "[Light Set] Warning: Spotlight not added because "
		             "array was full!"
		          << std::endl;
		return;
	}
	mSpotlights[mNumSpotlights++] = spotlight;
}

/**
 * Returns a struct that contains all data
 * @return A struct that contains all data and complies to glsl std140 layout rules
 */
LightSet::LightSetStruct LightSet::getStruct() const {
	LightSetStruct lightStruct =
	  LightSetStruct{{}, {}, {}, mNumDirLights, mNumPointLights, mNumSpotlights};
	for (int i = 0; i < mNumDirLights; i++)
		lightStruct.directionalLights[i] = mDirectionalLights.at(i)->getStruct();
	for (int i = 0; i < mNumPointLights; i++)
		lightStruct.pointLights[i] = mPointLights.at(i)->getStruct();
	for (int i = 0; i < mNumSpotlights; i++)
		lightStruct.spotlights[i] = mSpotlights.at(i)->getStruct();
	return lightStruct;
}
