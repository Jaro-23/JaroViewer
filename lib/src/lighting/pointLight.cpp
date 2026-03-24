#include "jaroViewer/lighting/pointLight.hpp"

using namespace JaroViewer;

PointLight::PointLight(Object obj, Tools::LightColor lightColor, Tools::AttenuationParams params)
  : mObject(obj),
    mLightColor(lightColor),
    mEnable(true),
    mConstant(params.constant),
    mLinear(params.linear),
    mQuadratic(params.quadratic) {}

void PointLight::enable(bool enable) { mEnable = enable; }

PointLight::PointLightStruct PointLight::getStruct() const {
	return PointLightStruct{mObject->getPosition(), mEnable,
	                        mLightColor.ambient,    mConstant,
	                        mLightColor.diffuse,    mLinear,
	                        mLightColor.specular,   mQuadratic};
}

Object PointLight::getObject() { return mObject; }
