#include "../../headers/entities/Cube2Tex.h"

Cube2Tex::Cube2Tex() {
	velocity = 0.0001f;
	offset[0] = offset[1] = 0.0f;
	//InitShader();
	//InitVBO1();
	//InitVAO1();
}

void Cube2Tex::InitShader() {
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

void Cube2Tex::InitVBO1() {
	static const float PI = 3.14f;
	static const float DPI = 6.28f;

	const float hside = 0.5f;
	Vertex Cube2Tex[] = {
		{{ -hside, hside, -hside }, { white }},
		{{ -hside, -hside, -hside }, { green }},
		{{  hside, -hside, -hside }, { blue }},
		{{  hside, hside, -hside }, { red }},
		{{ -hside, hside, hside }, { white }},
		{{ -hside, -hside, hside }, { green }},
		{{  hside, -hside, hside }, { blue }},
		{{  hside, hside, hside }, { red }},
	};

	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube2Tex), Cube2Tex, GL_STATIC_DRAW);

	GLuint indices[] = {
		0, 1, 2, 3,
		3, 2, 6, 7,
		7, 6, 5, 4,
		4, 5, 1, 0,
		4, 0, 3, 7,
		1, 5, 6, 2,
	};

	glGenBuffers(1, &IBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//checkOpenGLerror();
}

void Cube2Tex::InitVAO1() {
	glGenBuffers(1, &VAO1);

	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
	glEnableVertexAttribArray(Attrib_vertex);
	glEnableVertexAttribArray(Attrib_color);

	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_TRUE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(Attrib_vertex);
	glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_TRUE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(Attrib_color);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void Cube2Tex::InitVO() {
	InitVBO1();
	InitVAO1();
}

void Cube2Tex::ReleaseVO() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO1);
}

void Cube2Tex::Draw() {
	glUseProgram(Program);

	glUniform2f(glGetUniformLocation(Program, "offset"), offset[0], offset[1]);
	glBindVertexArray(VAO1);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	//checkOpenGLerror();
}