#include "../headers/pointLight.hpp"

using namespace JaroViewer;

PointLight::PointLight(Object obj, Tools::LightColor lightColor, Tools::AttenuationParams params)
  : Object(std::move(obj)),
    mLightColor{lightColor},
    mEnable{true},
    mConstant{params.constant},
    mLinear{params.linear},
    mQuadratic{params.quadratic} {}

void PointLight::enable(bool enable) { mEnable = enable; }

PointLight::PointLightStruct PointLight::getStruct() const {
	return PointLightStruct{mTranslation,         mEnable,
	                        mLightColor.ambient,  mConstant,
	                        mLightColor.diffuse,  mLinear,
	                        mLightColor.specular, mQuadratic};
}
