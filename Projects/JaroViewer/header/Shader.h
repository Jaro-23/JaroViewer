#pragma once

#include <glad/glad.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <vector>
#include <string>

namespace JaroViewer {
	class Shader {
		public:
			struct ShaderCode {
				const std::string *vertexCode;
				const std::string *geometryCode;
				const std::string *fragmentCode;
			};

			Shader(const ShaderCode &code);
			Shader(const std::vector<std::string> &vertexPaths, const std::vector<std::string> &fragmentPaths);
			Shader(const std::vector<std::string> &vertexPaths, const std::vector<std::string> &geometryPaths, const std::vector<std::string> &fragmentPaths);

			void use();
			
			void setBool(const std::string &name, bool value) const;
			void setInt(const std::string &name, int value) const;
			void setUniformBuffer(const std::string &name, int position) const;

			void setFloat1(const std::string &name, float x) const;
			void setFloat2(const std::string &name, float x, float y) const;
			void setFloat3(const std::string &name, float x, float y, float z) const;
			void setFloat4(const std::string &name, float x, float y, float z, float w) const;

			void setVec3(const std::string &name, glm::vec3 vec) const;
			void setMat3(const std::string &name, glm::mat3 mat) const;
			void setMat4(const std::string &name, glm::mat4 mat) const;

		private:
			void readFile(const std::string &filePath, std::string *out) const;
			unsigned int createShaderFromFile(GLenum shaderType, const std::vector<std::string> *sources, const std::string &errorName);
			unsigned int createShaderFromString(GLenum shaderType, const char* code, const std::string &errorName);
			void createProgram(unsigned int vertexID, unsigned int geometryID, unsigned int fragmentID);
			void checkCompilingError(unsigned int shaderID, const std::string &shaderName) const;
			void checkLinkingError(unsigned int programID) const;
			GLint getLocation(const std::string &name) const;

			unsigned int mProgramID;
	};
};
