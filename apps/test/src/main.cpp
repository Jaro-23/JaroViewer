#include "jaroViewer/rendering/shaderManager.hpp"
#include <iostream>
#include <jaroViewer/core/engine.hpp>
#include <jaroViewer/geometry/basicShapes.hpp>
#include <jaroViewer/graphics/materialManager.hpp>
#include <jaroViewer/modifiers/wavingModifier.hpp>

#include <memory>
#include <sys/types.h>

using namespace JaroViewer;

int main(int argc, char* argv[]) {
	EngineArgs args{};
	args.windowSamples = 16;
	args.cubemapParams = "./apps/test/cubemap";
	// args.postProcessShader = "./apps/test/fragment/postprocessing.fs";
	Engine engine{args};
	EngineState* state = engine.getState();

	// Create material and object
	ObjectManager& om   = state->objectManager;
	MaterialManager* mm = om.getMaterialManager();
	uint mat            = mm->createNew();
	mm->addMaterial(mat, {"./apps/test/textures/crate.jpg", "./apps/test/textures/crate_specular.jpg", 32.0f});
	om.registerModel("cube", cubeVertices, PredefinedShader::BASIC, mat);
	om.registerModel("backpack", "./apps/test/models/backpack/backpack.obj", PredefinedShader::BASIC);
	om.registerModel("light", cubeVertices, PredefinedShader::WHITE, 0);

	// Add the lights
	Tools::LightColor lightColor{glm::vec3(0.05f), glm::vec3(0.55f), glm::vec3(1.00f)};
	Tools::AttenuationParams attenParams{1.0f, 0.09f, 0.032f};
	std::shared_ptr<DirectionalLight> dir{
	  new DirectionalLight{glm::vec3(-0.2f, -1.0f, -0.3f), lightColor}
	};
	std::shared_ptr<PointLight> pointLight =
	  std::make_shared<PointLight>(om.createObject("light"), lightColor, attenParams);
	pointLight->getObject()->setTranslation(glm::vec3(-2.0f, 0.3f, 1.0f));
	pointLight->getObject()->setScale(0.2f);
	std::shared_ptr<PointLight> pointLight2 =
	  std::make_shared<PointLight>(om.createObject("light"), lightColor, attenParams);
	pointLight2->getObject()->setTranslation(glm::vec3(0.5f, 1.2f, -9.0f));
	pointLight2->getObject()->setScale(0.2f);

	state->lights.addDirLight(dir);
	state->lights.addPointLight(pointLight);
	state->lights.addPointLight(pointLight2);

	// Places "random" cubes
	std::array<glm::vec3, 10> cubePositions =
	  {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
	   glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
	   glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

	std::shared_ptr<WavingModifier> mod = std::make_shared<WavingModifier>();

	std::vector<Object> objs;
	for (int i = 0; i < cubePositions.size(); i++) {
		objs.push_back(om.createObject("cube"));
		objs.back()->addModifier(mod);
		objs.back()->setTranslation(cubePositions.at(i));
		objs.back()->setRotation(3.65f * i, 23.78f * i, 43.12f * i);
	}

	Object obj = om.createObject("backpack");
	obj->setScale(0.1f);
	obj->setTranslation(glm::vec3(0.0f, 0.0f, 2.0f));

	engine.start();
	return 0;
}
