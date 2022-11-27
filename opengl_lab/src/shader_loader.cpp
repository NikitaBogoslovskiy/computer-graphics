#define _CRT_SECURE_NO_WARNINGS
#include "../headers/shader_loader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

const char* ShaderLoader::loadShader(const GLchar* shaderPath) {
	// retrieve the shader/fragment source code from filePath
	std::string shaderCode;
	std::ifstream shaderFile;
	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		shaderFile.open(shaderPath);
		std::stringstream shaderStream, fShaderStream;
		// read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();
		// convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
	}

	// god forgive me for that
	const char* temp = shaderCode.c_str();
	char* dst = new char[strlen(temp) + 1];
	std::strcpy(dst, temp);
	return dst;
}

void ShaderLoader::checkCompileErrors(const GLuint& shader, const std::string& type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

const std::string ShaderLoader::shaderEnumToStr(GLenum type) {
	switch (type)
	{
	case GL_VERTEX_SHADER:
		return "VERTEX";
	case GL_GEOMETRY_SHADER:
		return "GEOMETRY";
	case GL_FRAGMENT_SHADER:
		return "FRAGMENT";
	default:
		return "<undefined>";
	}
}

GLuint ShaderLoader::compileShader(const GLchar* const* shaderCode, const GLenum& type) {
	GLuint shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, shaderCode, NULL);
	glCompileShader(shaderID);
	ShaderLoader::checkCompileErrors(shaderID, shaderEnumToStr(type));
	return shaderID;
}

GLuint ShaderLoader::createProgram(const GLuint& vShader, const GLuint& fShader) {
	// shader Program
	GLuint Program = glCreateProgram();
	glAttachShader(Program, vShader);
	glAttachShader(Program, fShader);
	glLinkProgram(Program);
	checkCompileErrors(Program, "PROGRAM");
	return Program;
}

GLuint ShaderLoader::initProgram(const char* vertexPath, const char* fragmentPath) {
	auto VertexShaderSource = ShaderLoader::loadShader(vertexPath);
	GLuint vShader = ShaderLoader::compileShader(&VertexShaderSource, GL_VERTEX_SHADER);

	auto FragShaderSource = ShaderLoader::loadShader(fragmentPath);
	GLuint fShader = ShaderLoader::compileShader(&FragShaderSource, GL_FRAGMENT_SHADER);

	GLuint Program = ShaderLoader::createProgram(vShader, fShader);

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	return Program;
}