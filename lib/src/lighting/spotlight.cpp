#include "jaroViewer/lighting/spotlight.hpp"

#include "glm/trigonometric.hpp"

using namespace JaroViewer;

Spotlight::Spotlight(
  Object obj,
  glm::vec3 direction,
  Tools::LightColor lightColor,
  Tools::AttenuationParams params,
  float innerAngle,
  float outerAngle
)
  : mObject{std::move(obj)},
    mLightColor{lightColor},
    mEnable{true},
    mDirection{direction},
    mCutOff{glm::cos(glm::radians(innerAngle))},
    mOuterCutOff{glm::cos(glm::radians(outerAngle))},
    mConstant{params.constant},
    mLinear{params.linear},
    mQuadratic{params.quadratic} {}

void Spotlight::enable(bool enable) { mEnable = enable; }
bool Spotlight::getState() const { return mEnable; }

Spotlight::SpotlightStruct Spotlight::getStruct() const {
	return SpotlightStruct{
	  mObject->getPosition(),
	  mCutOff,
	  mDirection,
	  mOuterCutOff,
	  mLightColor.ambient,
	  mConstant,
	  mLightColor.diffuse,
	  mLinear,
	  mLightColor.specular,
	  mQuadratic,
	  mEnable,
	  0.0f,
	  0.0f,
	  0.0f
	};
}

Object Spotlight::getObject() { return mObject; }

void Spotlight::setDirection(const glm::vec3& direction) {
	mDirection = direction;
}
