#include "../../headers/entities/ColoredEllipse.h"

ColoredEllipse::ColoredEllipse() {
	velocity = 0.0001;
	InitShader();
	InitVO();
}

void ColoredEllipse::InitShader() {
	Program = ShaderLoader::initProgram("shaders/task4/ellipse.vert", "shaders/task4/ellipse.frag");

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
	//checkOpenGLerror();
}

void ColoredEllipse::InitVBO() {
	Vertex ellipse[362];

	ellipse[0] = { {0.f, 0.f, 0.f}, {white} };

	float ddpi = DPI / 360;
	float sphi = 0.f;
	for (size_t i = 1; i < 362; sphi += ddpi, i++)
	{
		ellipse[i] = { { cosf(sphi), sinf(sphi), 0.f}, { __r(sphi), __g(sphi), __b(sphi), 1.f } };
	}

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse), ellipse, GL_STATIC_DRAW);
}

void ColoredEllipse::InitVO() {
	glGenVertexArrays(1, &VAO);

	// THE RIGHT ORDER IS bind the VAO first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	InitVBO();
	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(Attrib_vertex);
	glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(Attrib_color);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ColoredEllipse::ReleaseVO() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
}

void ColoredEllipse::Draw(const float& time) {
	glUseProgram(Program);
	glUniform2f(glGetUniformLocation(Program, "scale"), scale[0], scale[1]);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 362);
	glBindVertexArray(0);
	glUseProgram(0);

	//checkOpenGLerror();
}