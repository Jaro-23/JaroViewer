#include <Shader.h>
#include <Engine3D.h>
#include <BoxComponent.h>

int main() {
	JaroViewer::Camera camera = JaroViewer::Camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera.setSpeed(0.1f);
	JaroViewer::Engine3D engine = JaroViewer::Engine3D(3, 600, 450, "Basic example", &camera);

	JaroViewer::Shader shader{{"/vertex/Transform.vs", "/vertex/Wireframe.vs"}, {"/fragment/Wireframe.fs"}};
	JaroViewer::BoxComponent box{shader};
	box.setWireframeMode(true);
	engine.addComponent(&box);

	engine.start();
}
