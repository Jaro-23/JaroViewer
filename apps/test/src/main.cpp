#include "basicShaders.hpp"
#include "MaterialManager.hpp"
#include <engine.hpp>
#include <sys/types.h>

using namespace JaroViewer;

int main(int argc, char* argv[]) {
	EngineArgs args{};
	Engine engine{args};
	EngineState* state = engine.getState();

	// Create material and object
	ObjectManager& om   = state->objectManager;
	MaterialManager* mm = om.getMaterialManager();
	uint mat            = mm->createNew();
	mm->addMaterial(mat, {"./apps/test/textures/crate.jpg", "./apps/test/textures/crate_specular.jpg", 32.0f});

	om.registerModel(
	  "cube",
	  {
	    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  -0.5f, -0.5f,
	    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,
	    0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,

	    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
	    -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
	    1.0f,  0.0f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	    0.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	    0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  0.0f,

	    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,  0.5f,  -0.5f,
	    -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
	    0.0f,  -1.0f, 1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
	    0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,
	    0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,

	    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
	    0.5f,  0.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,
	    -1.0f, 0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
	    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
	    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f,  0.0f,

	    0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
	    0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,
	    0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,
	    0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f,
	    -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f,
	    0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,
	    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	  },
	  (uint)1, mat
	);
	std::array<glm::vec3, 10> cubePositions =
	  {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
	   glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
	   glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

	std::vector<Object> objs = {};
	for (int i = 0; i < cubePositions.size(); i++) {
		objs.push_back(om.createObject("cube"));
		objs.back().setTranslation(cubePositions.at(i));
		objs.back().setRotation(3.65f * i, 23.78f * i, 43.12 * i);
	}

	engine.start();
	return 0;
}
