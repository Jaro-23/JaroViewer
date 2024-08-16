#include "../header/DirectionalLight.h"

using namespace JaroViewer;

/**
 * Creates a directional light
 * @param direction The direction of the light
 * @param ambient The ambient color of the light
 * @param diffuse The diffuse color of the light
 * @param specular The specular color of the light
 */
DirectionalLight::DirectionalLight(glm::vec3 direction, Tools::LightColor lightColor) : 
	mDirection{direction},
	mLightColor{lightColor},
	mEnable{true}
{

}

/**
 * Enables or disables the light
 * @param enable New enable state of the light
 */
void DirectionalLight::enable(bool enable) { mEnable = enable; }

/**
 * Returns a struct with all data
 * @return A struct with all data following glsl std140 layout rules
 */
DirectionalLight::DirectionalLightStruct DirectionalLight::getStruct() const {
	return DirectionalLightStruct{
		mDirection,
		mEnable,
		mLightColor.ambient,
		0,
		mLightColor.diffuse,
		0,
		mLightColor.specular,
		0
	};
}
