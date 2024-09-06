#include "../header/PostProcessor.h"
#include "../header/Tools.h"
#include <memory>

using namespace JaroViewer;

const std::string vertexCode = 
	"#version 420 core\n"
	"layout (location = 0) in vec2 aPos;\n"
	"layout (location = 1) in vec2 aTexCoords;\n"
	"out vec2 TexCoords;\n"
	"void main() {\n"
	"	gl_Position = vec4(aPos, 0.0, 1.0);\n"
	"	TexCoords = aTexCoords;\n\n"
	"}\n"
;

PostProcessor::PostProcessor(int width, int height, const std::string fragmentFile) :
	mFrameBuffer{width, height, true, false}
{
	std::string buff;
	Tools::readFile(fragmentFile, &buff);
	mShader = std::make_unique<Shader>(Shader::ShaderCode{&vertexCode, nullptr, &buff});
	setupVao();
}

void PostProcessor::setupVao() {
	std::vector<float> vertices{
		-1.0f, 	1.0f,
		 1.0f, -1.0f,
		 1.0f, 	1.0f,

		-1.0f, 	1.0f,
		-1.0f, -1.0f,
		 1.0f, -1.0f,
	};	

	glGenVertexArrays(1, &mVaoBuffer);
	glBindVertexArray(mVaoBuffer);
	
	Tools::generateBuffer(vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void PostProcessor::bind() const {
	//mFrameBuffer.bind();	
}

void PostProcessor::unbind() const {
	//mFrameBuffer.unbind();
}

void PostProcessor::clear(float r, float g, float b, float a) const {
	glClearColor(r, g, b, a);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessor::render() const {
	//mFrameBuffer.unbind();
	mShader->use();
	glBindVertexArray(mVaoBuffer);
	//glBindTexture(GL_TEXTURE_2D, mFrameBuffer.getTexture());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
