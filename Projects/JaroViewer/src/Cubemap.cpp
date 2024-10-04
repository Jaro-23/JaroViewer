#include "../header/Cubemap.h"

#include "../header/Tools.h"

#include <array>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB_IMAGE/stb_image.h>
#include <iostream>
#include <vector>
#include <filesystem>

using namespace JaroViewer;

// TODO : Rework the view because in needs 2 cast for each call
const std::string vertex =
	"#version 420 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"out vec3 TexCoords;\n"
	"layout(std140, binding = 0) uniform Transformation {\n"
	"	mat4 projection;\n"
	"	mat4 view;\n"
	"};\n"
	"void main() {\n"
	"    TexCoords = aPos;\n"
	"    gl_Position = (projection * mat4(mat3(view)) * vec4(aPos, 1.0)).xyww;\n"
	"}\n"
;

const std::string fragment =
	"#version 420 core\n"
	"out vec4 FragColor;\n"
	"in vec3 TexCoords;\n"
	"uniform samplerCube skybox;\n"
	"void main() {\n"
	"    FragColor = texture(skybox, TexCoords);\n"
	"}\n"
;

/**
 * Creates a cubemap based of the paths to the textures
 * @param paths The paths to the texture in order (right, left, top, bottom, front, back);
 */
Cubemap::Cubemap(const std::vector<std::string> &paths) :
	mShader{Shader::ShaderCode{&vertex, nullptr, &fragment}}
{
	loadVao();
	loadMap(paths);
}

/**
 * Creates a cubemap based of the path to the directory with all textures
 * @param dir The directory with all the textures
 */
Cubemap::Cubemap(const std::string &dir) :
	mShader{Shader::ShaderCode{&vertex, nullptr, &fragment}}
{
	std::vector<std::string> paths;
	getPaths(dir, &paths);
	loadVao();
	loadMap(paths);
}

/**
 * Renders the cubemap
 */
void Cubemap::render() const {
	if (mVaoBuffer == 0) return;
	glDepthFunc(GL_LEQUAL);
	mShader.use();
	glBindVertexArray(mVaoBuffer);
	bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}

// Binds the cubemap to it's active spot
void Cubemap::bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, mCubemap); }

// Unbinds all textures from the cubemap spot
void Cubemap::unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

void Cubemap::loadVao() {
	std::vector<float> vertices = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &mVaoBuffer);
	glBindVertexArray(mVaoBuffer);

	Tools::generateBuffer(vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

/**
 * Given a dir it will populate the out vector with the six sides of the cubemap in order
 * @param dir The directory containing the textures
 * @param out The output that will be populated
 */
void Cubemap::getPaths(const std::string dir, std::vector<std::string> *out) const {
	std::string names[6] = {"right", "left", "top", "bottom", "front", "back"};
	for (int i = 0; i < 6; i++) {
		bool found = false;
		for (const auto & entry : std::filesystem::directory_iterator(dir)) {
			std::string path = entry.path();
			int index = path.find_last_of('/') + 1;
			std::string name = path.substr(index, path.length() - index);
			if (name.rfind(names[i] + ".", 0) == 0) {
				found = true;
				out->push_back(path);
				break;
			}
		}
	}
}

/**
 * Loads a cubemap texture into a new buffer
 * @param paths The paths to the texture in order (right, left, top, bottom, front, back);
 */
void Cubemap::loadMap(const std::vector<std::string> &paths) {
	genBuffer();
	bind();
	bindTextures(paths);
	setupParams();
}

// Generates a new cubemap texture
void Cubemap::genBuffer() {	glGenTextures(1, &mCubemap); }

/**
 * Binds all paths to the texture
 * @param paths The paths to the texture in order (right, left, top, bottom, front, back);
 * @pre The cubemap is active
 */
void Cubemap::bindTextures(const std::vector<std::string> &paths) {
	int width, height, nrChannels;
	for (unsigned int i = 0; i < paths.size(); i++) {
		unsigned char *data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			std::cout << "ERROR::CUBEMAP::UNABLE_TO_LOAD::" << paths[i] << std::endl;
		stbi_image_free(data);
	}
}

/**
 * Sets all the filter and wrap parameters
 * @pre The cubemap is active
 */
void Cubemap::setupParams() const {
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
