#include "../../headers/entities/Tetrahedron.h"

Tetrahedron::Tetrahedron() {
	InitShader();
	InitVO();
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

void Tetrahedron::InitVBO() {
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

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedron), tetrahedron, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//checkOpenGLerror();
}

void Tetrahedron::InitVO() {
	glGenVertexArrays(1, &VAO);

	// THE RIGHT ORDER IS bind the VAO first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	InitVBO();
	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(Attrib_vertex);
	glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); //7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))
	glEnableVertexAttribArray(Attrib_color);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Tetrahedron::ReleaseVO() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &IBO);
}

void Tetrahedron::Draw(const float& time) {

	glUseProgram(Program);
	glUniform2f(glGetUniformLocation(Program, "offset"), offset[0], offset[1]);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	
	//checkOpenGLerror();
}