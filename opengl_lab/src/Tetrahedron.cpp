#include "../headers/Tetrahedron.h"

Tetrahedron::Tetrahedron() {
	velocity = 0.0001f;
	offset[0] = offset[1] = 0.0f;
	InitShader();
	InitVBO1();
	InitVAO1();
}

void Tetrahedron::InitShader() {
	Program = ShaderLoader::initProgram("shaders/task1/tetrahedron.vert", "shaders/task1/tetrahedron.frag");

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

void Tetrahedron::InitVBO1() {
	static const float PI = 3.14f;
	static const float DPI = 6.28f;

	Vertex tetrahedron[] = {
{{ 10.2606 / 55.f, 45.9957 / 55.f - 0.5f, -31.4461 / 55.f }, { white }},
{{ -28.1908 / 55.f, 7.32734 / 55.f - 0.5f, -6.42109 / 55.f }, { green }},
{{ -10.2606 / 55.f, 55.7862 / 55.f - 0.5f, 24.0789 / 55.f }, { blue }},
{{ 28.1908 / 55.f, 10.8908 / 55.f - 0.5f, 13.7884 / 55.f }, { red }}
	};

	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedron), tetrahedron, GL_STATIC_DRAW);

	GLuint indices[] = {
		0, 1, 2,
		0, 1, 3,
		0, 2, 3,
		1, 2, 3,
	};

	glGenBuffers(1, &IBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//checkOpenGLerror();
}

void Tetrahedron::InitVAO1() {
	glGenBuffers(1, &VAO1);

	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
	glEnableVertexAttribArray(Attrib_vertex);
	glEnableVertexAttribArray(Attrib_color);

	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(Attrib_vertex);
	glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(Attrib_color);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void Tetrahedron::InitVO() {
	InitVBO1();
	InitVAO1();
}

void Tetrahedron::ReleaseVO() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO1);
}

void Tetrahedron::Draw() {
	glUseProgram(Program);

	glUniform2f(glGetUniformLocation(Program, "offset"), offset[0], offset[1]);
	glBindVertexArray(VAO1);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	//checkOpenGLerror();
}