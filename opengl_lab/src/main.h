#pragma once
#ifndef __MAIN_H__
#define __MAIN_H__

#include "gl/glew.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#define red 1.0,0.0,0.0,1.0
#define green 0.0,1.0,0.0,1.0
#define blue 0.0,0.0,1.0,1.0
#define yellow 1.0,1.0,0.3,1.0
#define white 1.0,1.0,1.0,1.0

// Исходный код вершинного шейдера
const char* VertexShaderSource = R"(
#version 330 core

in vec3 coord;
in vec4 color;

out vec4 vcolor;

uniform vec2 offset;

void main() {
    gl_Position = vec4(coord.xy + offset, coord.z, 1.2);
    vcolor = color;
}
)";

const char* FragShaderSource = R"(
#version 330 core

in vec4 vcolor;

out vec4 FragColor;

void main() {
    FragColor = vcolor;
}
)";


GLuint Program;

GLuint Attrib_vertex;
GLuint Attrib_color;
GLuint Attrib_texture;

GLuint VBO1;
GLuint IBO1;
GLuint VAO1;

GLuint VBO2;
GLuint VAO2;

GLfloat offset[2] = { 0, 0 };
GLfloat scale[2] = { 1, 1 };
bool is_left = false,
is_right = false,
is_up = false,
is_down = false;

struct Vertex {
	struct Coord {
		GLfloat x;
		GLfloat y;
		GLfloat z;
	} coord;
	struct Color {
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
	} color;
};

sf::Shader shader;

void checkOpenGLerror() {

}

int current_task = 1;

void ShaderLog(unsigned int shader)
{
	int infologLen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
	if (infologLen > 1)
	{
		int charsWritten = 0;
		std::vector<char> infoLog(infologLen);
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
		std::cout << "InfoLog: " << infoLog.data() << std::endl;
	}
}

void InitShader() {
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &VertexShaderSource, NULL);
	glCompileShader(vShader);
	std::cout << "vertex shader \n";
	ShaderLog(vShader);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &FragShaderSource, NULL);
	glCompileShader(fShader);
	std::cout << "fragment shader \n";
	ShaderLog(fShader);
	Program = glCreateProgram();
	glAttachShader(Program, vShader);
	glAttachShader(Program, fShader);
	glLinkProgram(Program);
	int link_ok;
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		std::cout << "error attach shaders \n";
		return;
	}
	// Вытягиваем ID атрибута из собранной программы
	const char* attr_name = "coord"; //имя в шейдере
	Attrib_vertex = glGetAttribLocation(Program, attr_name);
	if (Attrib_vertex == -1) {
		std::cout << "could not bind attrib " << attr_name << std::endl;
		return;
	}

	const char* attr_name2 = "color"; //имя в шейдере
	Attrib_color = glGetAttribLocation(Program, attr_name2);
	if (Attrib_color == -1) {
		std::cout << "could not bind attrib " << attr_name2 << std::endl;
		return;
	}

	checkOpenGLerror();
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int width, height, nrChannels;
unsigned char* data;

static constexpr float PI = 3.14159265359f;
static constexpr float DPI = PI * 2;

static constexpr float a60 = PI / 3;
static constexpr float a120 = a60 * 2;
static constexpr float a180 = PI;
static constexpr float a240 = PI + a60;
static constexpr float a300 = PI + a120;
static constexpr float a360 = DPI;


float __r(float phi) {
	if (phi <= a60 || phi >= a300) return 1.f;
	if (phi >= a120 && phi <= a240) return 0.f;
	if (phi < a120) return 1.f - (phi - a60) / a60;
	if (phi > a240) return (phi - a240) / a60;
}

float __g(float phi) {
	if (phi >= a60 && phi <= a180) return 1.f;
	if (phi >= a240) return 0.f;
	if (phi < a60) return phi / a60;
	if (phi > a180) return 1.f - (phi - a180) / a60;
}

float __b(float phi) {
	return __g(DPI - phi);
}

void InitVBO1() {
	Vertex tetrahedron[4] = {
{{ -28.1908 / 55.f, 7.32734 / 55.f - 0.5f, -6.42109 / 55.f }, { green }},
{{ -10.2606 / 55.f, 55.7862 / 55.f - 0.5f, 24.0789 / 55.f }, { blue }},
{{ 28.1908 / 55.f, 10.8908 / 55.f - 0.5f, 13.7884 / 55.f }, { red }},
{{ 10.2606 / 55.f, 45.9957 / 55.f - 0.5f, -31.4461 / 55.f }, { white }},
	};
	GLuint indices[12] = {
		0, 1, 2,
		0, 1, 3,
		0, 2, 3,
		1, 2, 3,
	};

	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &IBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedron), tetrahedron, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void InitVAO1() {
	glGenVertexArrays(1, &VAO1);

	// THE RIGHT ORDER IS bind the VAO first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO1);

	InitVBO1();
	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(Attrib_vertex);
	glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); //7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))
	glEnableVertexAttribArray(Attrib_color);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void InitVBO2() {
	Vertex ellipse[360];

	ellipse[0] = { {0.f, 0.f, 0.f}, {white} };

	float ddpi = DPI / 360;
	float sphi = 0.f;
	for (size_t i = 1; i <= 359; sphi += ddpi, i++)
	{
		ellipse[i] = { { cosf(sphi), sinf(sphi), 0.f}, { __r(sphi), __g(sphi), __b(sphi), 1.f } };
	}

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse), ellipse, GL_STATIC_DRAW);
}
void InitVAO2() {
	glGenVertexArrays(1, &VAO2);

	// THE RIGHT ORDER IS bind the VAO first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO2);

	InitVBO2();
	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(Attrib_vertex);
	glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(Attrib_color);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Init() {
	InitShader();
	InitVAO1();
	InitVAO2();
}

void Draw() {
	glUseProgram(Program);

	glUniform2f(glGetUniformLocation(Program, "offset"), offset[0], offset[1]);

	switch (current_task)
	{
	case 1:
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		break;
	case 2:
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 360);
		glBindVertexArray(0);
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}

	glUseProgram(0);
	checkOpenGLerror();
}

void ReleaseShader() {
	// Передавая ноль, мы отключаем шейдерную программу
	glUseProgram(0);
	// Удаляем шейдерную программу
	glDeleteProgram(Program);

}

void ReleaseVBO() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VAO1);
	glDeleteBuffers(1, &VAO2);
	glDeleteBuffers(1, &IBO1);
}

void Release() {
	// Шейдеры
	ReleaseShader();
	// Вершинный буфер
	ReleaseVBO();
}


void SetIcon(sf::Window& wnd)
{
	sf::Image image;
	// Вместо рисования пикселей, можно загрузить иконку из файла (image.loadFromFile("icon.png"))
	image.create(16, 16);
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
			image.setPixel(i, j, { (uint8_t)(i * 16), (uint8_t)(j * 16), 0 });

	wnd.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}

#endif // !__MAIN_H__
