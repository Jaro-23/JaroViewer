#include "jaroViewer/rendering/postProcessor.hpp"
#include "jaroViewer/core/tools.hpp"
#include "jaroViewer/rendering/basicShaders.hpp"

#include <memory>

using namespace JaroViewer;

const std::string vertexCode = shaderVersion +
  "layout (location = 0) in vec2 aPos;\n"
  "layout (location = 1) in vec2 aTexCoords;\n"
  "out vec2 TexCoords;\n"
  "void main() {\n"
  "	gl_Position = vec4(aPos, 0.0, 1.0);\n"
  "	TexCoords = aTexCoords;\n\n"
  "}\n";

/**
 * Creates a postprocessor
 * @param window The window on which the postprocessor will be applied
 * @param fragmentFile The path to the fragment shader for the postprocessor
 */
PostProcessor::PostProcessor(Window* window, const std::string fragmentFile)
  : mFrameBuffer({window->getSize().width, window->getSize().height, true, false, GL_RGB}) {
	std::string buff;
	Tools::readFile(fragmentFile, &buff);
	mShader = std::make_unique<Shader>(ShaderCode{vertexCode, "", buff});
	setupVao();
}

/**
 * Creates a vao for a plane that covers the screen
 */
void PostProcessor::setupVao() {
	std::vector<float> vertices{
	  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
	  -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &mVaoBuffer);
	glBindVertexArray(mVaoBuffer);

	Tools::generateBuffer(vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

/**
 * Binds the postprocessor so it gets drawn on
 */
void PostProcessor::bindAndClear(float r, float g, float b, float a) const {
	mFrameBuffer.bind();
	mFrameBuffer.clear(r, g, b, a);
}

/**
 * Unbinds the postprocessor so the screen gets drawn on
 */
void PostProcessor::unbind() const { mFrameBuffer.unbind(); }

/**
 * Renders the postprocessor with the fragment shader to the screen
 */
void PostProcessor::render() const {
	mFrameBuffer.unbind();
	mShader->use();
	mShader->setInt("screenTexture", 0);
	glBindVertexArray(mVaoBuffer);
	glActiveTexture(GL_TEXTURE0);
	unsigned int texture = mFrameBuffer.getColor();
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
