#include <Engine.hpp>

using namespace JaroViewer;

int main(int argc, char* argv[]) {
	EngineArgs args{};
	Engine engine{args};
	engine.start();
	return 0;
}
