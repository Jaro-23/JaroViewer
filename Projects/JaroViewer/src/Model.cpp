#include "../header/Model.h"
#include "../header/ModelFactory.h"
#include <iostream>

using namespace JaroViewer;

Model::Model(Shader &shader, const std::string &path) : 
	Component3D{shader},
	mPath{path}
{ 
	mVaoBuffer = -1; // Set the vao buffer to value so it passes the check at start of render
}

void Model::load() {}

void Model::wireframeRender(const RenderData &data) {}

void Model::defaultRender(const RenderData &data) {	
	ModelFactory::getInstance()->drawModel(mPath, getModelMatrix(), data, mShader);
}
