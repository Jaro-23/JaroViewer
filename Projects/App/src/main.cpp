#include "GLM/trigonometric.hpp"
#include "PointLight.h"
#include <Shader.h>
#include <Engine3D.h>
#include <BoxComponent.h>

#include <GLM/gtx/string_cast.hpp>
#include <memory>

using namespace JaroViewer;

int main() {
	// Always create engine first because it will start glfw etc..
	Camera camera = Camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera.setSpeed(3.5f);
	Engine3D engine = Engine3D(Window(3, 600, 450, "Basic example"), &camera);
	
	// Create some shaders and textures
	const std::string basePath = "./Projects/App/";
	Shader shader{{basePath + "vertex/Library.vs", basePath + "vertex/Basic.vs"}, {basePath + "fragment/Library.fs", basePath + "fragment/Basic.fs"}};
	Shader fullWhiteShader{{basePath + "vertex/Library.vs", basePath + "vertex/FullWhite.vs"}, {basePath + "fragment/FullWhite.fs"}};
	Shader wireframeShader{{basePath + "vertex/Wireframe.vs"}, {basePath + "geometry/Wireframe.gs"}, {basePath + "fragment/Wireframe.fs"}};
	Material crate{basePath + "textures/crate.jpg", basePath + "textures/crate_specular.jpg", 32.0f};
	Tools::LightColor lightColor{glm::vec3(0.05f), glm::vec3(0.55f), glm::vec3(1.00f)};
	Tools::AttenuationParams attenParams{ 1.0f, 0.09f, 0.032f };

	// Create the lightset and add it
	std::shared_ptr<DirectionalLight> dir{ new DirectionalLight{ glm::vec3(-0.2f, -1.0f, -0.3f), lightColor} };
	std::shared_ptr<PointLight> pointLight{ new PointLight{ fullWhiteShader, wireframeShader, lightColor, attenParams } };
	pointLight->setTranslation(glm::vec3(-2.0f, 0.3f, 1.0f));
	std::shared_ptr<PointLight> pointLight2{ new PointLight{ fullWhiteShader, wireframeShader, lightColor, attenParams } };
	pointLight2->setTranslation(glm::vec3(0.5f, 1.2f, -9.0f));
	std::shared_ptr<Spotlight> flashlight{ new Spotlight{ fullWhiteShader, wireframeShader, glm::vec3{1.0f}, lightColor, attenParams, 12.5f, 17.5f } };
	camera.setFlashlight(flashlight);

	LightSet light{};
	light.addDirLight(dir);
	light.addPointLight(pointLight);
	light.addPointLight(pointLight2);
	light.addSpotlight(flashlight);

	engine.setLightSet(&light);
	engine.addComponent(pointLight);
	engine.addComponent(pointLight2);
	
	// Create a few crates spaced around
	std::vector<glm::vec3> cubePositions = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	for (int i = 0; i < cubePositions.size(); i++) {
		std::shared_ptr<Component3D> box{new BoxComponent(shader, wireframeShader)};
		box->addMaterial(crate);

		box->setTranslation(cubePositions.at(i));
		box->setRotation(3.65f * i, 23.78f * i, 43.12 * i);

		engine.addComponent(box);
	}

	// Start the engine
	engine.start();
}
