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
	mLightColor{lightColor}
{

}

/**
 * Returns a struct with all data
 * @return A struct with all data following glsl std140 layout rules
 */
DirectionalLight::DirectionalLightStruct DirectionalLight::getStruct() const {
	return DirectionalLightStruct{
		mDirection,
		0,
		mLightColor.ambient,
		0,
		mLightColor.diffuse,
		0,
		mLightColor.specular,
		0
	};
}
