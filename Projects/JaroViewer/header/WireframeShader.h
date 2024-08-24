#pragma once

#include "Shader.h"

namespace JaroViewer {
	class WireframeShader {
		public:
			static Shader getShader() { 
				Shader::ShaderCode code{&vertex, &geometry, &fragment};
				return Shader(code); 
			}

		private:
			static std::string vertex;
			static std::string geometry;
			static std::string fragment;
	};
}

std::string JaroViewer::WireframeShader::vertex =
	"#version 420 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aNormal;\n"
	"out VS_OUT {\n"
	"	vec3 Normal;\n"
	"} vs_out;\n"
	"void main() {\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"	vs_out.Normal = aNormal;\n"
	"}\n"
;

std::string JaroViewer::WireframeShader::geometry = 
	"#version 420 core\n"
	"layout (triangles) in;\n"
	"layout (line_strip, max_vertices = 10) out;\n"
	"#define MAGNITUDE 0.4\n"
	"layout(std140) uniform Transformation {\n"
	"	mat4 projection;\n"
	"	mat4 view;\n"
	"};\n"
	"uniform mat4 model;\n"
	"in VS_OUT {\n"
	"	vec3 Normal;\n"
	"} gs_in[];\n"
	"out vec4 Color;\n"
	"void drawNormal(vec3 normal, int index) {\n"
	"	Color = vec4(1.0, 0.0, 0.0, 1.0);\n"
	"	gl_Position = projection * view * model * gl_in[index].gl_Position;\n"
	"	EmitVertex();\n"
	"	gl_Position = projection * view * model * (gl_in[index].gl_Position + vec4(normal, 0.0) * MAGNITUDE);\n"
	"	EmitVertex();\n"
	"	EndPrimitive();\n"
	"}\n"
	"void main() {\n"
	"	Color = vec4(1.0, 1.0, 1.0, 1.0);	\n"
	"	gl_Position = projection * view * model * gl_in[0].gl_Position;\n"
	"	EmitVertex();\n"
	"	gl_Position = projection * view * model * gl_in[1].gl_Position;\n"
	"	EmitVertex();\n"
	"	gl_Position = projection * view * model * gl_in[2].gl_Position;\n"
	"	EmitVertex();\n"
	"	gl_Position = projection * view * model * gl_in[0].gl_Position;\n"
	"	EmitVertex();\n"
	"	EndPrimitive();\n"
	"	drawNormal(gs_in[0].Normal, 0);\n"
	"	drawNormal(gs_in[1].Normal, 1);\n"
	"	drawNormal(gs_in[2].Normal, 2);\n"
	"}\n"
;

std::string JaroViewer::WireframeShader::fragment = 
	"#version 420 core\n"
	"in vec4 Color;\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"	FragColor = Color;\n"
	"}\n"
;
