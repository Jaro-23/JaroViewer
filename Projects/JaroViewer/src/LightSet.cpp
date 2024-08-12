#include "../header/LightSet.h"
#include <iostream>
#include <memory>

using namespace JaroViewer;

/**
 * Creates a lightset containing a dirlight and more
 * @param dirLight A shared pointer that point to a directional light
 */
LightSet::LightSet(std::shared_ptr<DirectionalLight> dirLight) :
	mDirectionalLight{dirLight},
	mPointLights{std::array<std::shared_ptr<PointLight>, mMAXNUMPOINTLIGHTS>()},
	mNumPointLights{0}
{

}

void LightSet::addPointLight(std::shared_ptr<PointLight> pointLight) {
	if (mNumPointLights >= mMAXNUMPOINTLIGHTS) {
		std::cout << "WARNING::LIGHTSET::POINTLIGHT_NOT_ADDED_FULL_ARRAY" << std::endl;
		return;
	}
	mPointLights[mNumPointLights++] = pointLight;
}

/**
 * Returns a struct that contains all data
 * @return A struct that contains all data and complies to glsl std140 layout rules
 */
LightSet::LightSetStruct LightSet::getStruct() const {
	LightSetStruct lightStruct = LightSetStruct{
		mDirectionalLight->getStruct(),
		{},
		mNumPointLights
	};
	for (int i = 0; i < mNumPointLights; i++) {
		lightStruct.pointLights[i] = mPointLights.at(i)->getStruct();	
	}
	return lightStruct;
}

/**
 * Returns a pointer to the directional light of the light set
 * @return A pointer to a directional light
 */
DirectionalLight* LightSet::getDirectionalLight() const { return &*mDirectionalLight; }
