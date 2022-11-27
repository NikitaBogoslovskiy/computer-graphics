#pragma once

#include "gl/glew.h"
#include <string>

class ShaderLoader {
private:
	static void checkCompileErrors(const GLuint& shader, const std::string& type);
	static const std::string shaderEnumToStr(GLenum type);
	static const char* loadShader(const char* shaderPath);
	static GLuint compileShader(const GLchar* const* shaderCode, const GLenum& type);
	static GLuint createProgram(const GLuint& vShader, const GLuint& fShader);

public:
	static GLuint initProgram(const char* vertexPath, const char* fragmentPath);
};
