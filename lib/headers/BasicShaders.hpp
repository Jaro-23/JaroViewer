#pragma once

#include <string>

namespace JaroViewer {
	const std::string basicWhiteVertex =
	  "#version 460 core\n"
	  "uniform mat4 model;\n"
	  "layout(std140, binding = 0) uniform Transformation {\n"
	  "mat4 projection;\n"
	  "mat4 view;\n"
	  "};\n"
	  "vec4 transform(vec3 pos) {\n"
	  "return projection * view * model * vec4(pos, 1.0);\n"
	  "}\n"
	  "layout (location = 0) in vec3 aPos;\n"
	  "layout (location = 1) in vec3 aNormal;\n"
	  "layout (location = 2) in vec2 aTexCoord;\n"
	  "void main() {\n"
	  "gl_Position = transform(aPos);\n"
	  "}\n";

	const std::string basicWhiteFragment = "#version 460 core\n"
	                                       "out vec4 FragColor;\n"
	                                       "void main() {\n"
	                                       "FragColor = vec4(1.0);\n"
	                                       "}\n";
} // namespace JaroViewer
