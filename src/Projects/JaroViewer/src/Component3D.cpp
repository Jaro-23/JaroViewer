#include "../header/Component3D.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <GLM/gtx/string_cast.hpp>

using namespace JaroViewer;

Component3D::Component3D(const Shader &shader) :
	mShader{ shader },
	mWireframeShader{
		Shader(
			std::vector<std::string>{"/vertex/Library.vs", "/vertex/Wireframe.vs"},
			std::vector<std::string>{"/fragment/Wireframe.fs"}
		)
	},
	mWireframeMode{false}
{
	mWireframeShader.setUniformBuffer("Matrices", 0);
}

void Component3D::setWireframeMode(bool enable) { mWireframeMode = enable; }

void Component3D::render(const RenderData &data) {
	if (mVaoBuffer == 0) return;

	glBindVertexArray(mVaoBuffer);
	if (mWireframeMode) {
		mWireframeShader.use();
		mWireframeShader.setMat4("model", data.model);	
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, mNumLines, GL_UNSIGNED_INT, 0);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		defaultRender(data);
	}
}

// Protected methods
bool Component3D::getWireframeMode() const { return mWireframeMode; }

// Private methods

void Component3D::defaultRender(const RenderData &data) {
	mShader.use();
	mShader.setMat4("model", data.model);
	glDrawElements(GL_TRIANGLES, mNumLines, GL_UNSIGNED_INT, 0);
}	
