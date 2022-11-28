#include "../../headers/entities/Cube3Tex.h"
#include "stb_image.h"
Cube3Tex::Cube3Tex() {
}

void Cube3Tex::InitShader() {
	Program = ShaderLoader::initProgram("shaders/task3/tt_cube.vert", "shaders/task3/tt_cube.frag");
	//Program = ShaderLoader::initProgram("shaders/task2/nr_cube.vert", "shaders/task2/nr_cube.frag");
	Attrib_vertex = 0;
	Attrib_color = 1;
	Attrib_texture = 2;
}

void Cube3Tex::InitVBO1() {

	const float hside = 0.5f;
	Vertex vertices[] = {
		/*{{ -hside, hside, -hside }, { white }},
		{{ -hside, -hside, -hside }, { green }},
		{{  hside, -hside, -hside }, { blue }},
		{{  hside, hside, -hside }, { red }},
		{{ -hside, hside, hside }, { white }},
		{{ -hside, -hside, hside }, { green }},
		{{  hside, -hside, hside }, { blue }},
		{{  hside, hside, hside }, { red }},*/
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint indices[] = {
		/*
		* quads
		0, 1, 2, 3,
		3, 2, 6, 7,
		7, 6, 5, 4,
		4, 5, 1, 0,
		4, 0, 3, 7,
		1, 5, 6, 2,*/

		/*
		triangles
		0, 1, 2,
		0, 2, 3,

		3, 2, 6,
		3, 6, 7,

		7, 6, 5,
		7, 5, 4,

		4, 5, 1,
		4, 1, 0,

		4, 0, 3,
		4, 3, 7,

		5, 1, 2,
		5, 2, 6*/

		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//checkOpenGLerror();
}

void Cube3Tex::InitVAO1() {
	glGenBuffers(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//glEnableVertexAttribArray(Attrib_vertex);
	//glEnableVertexAttribArray(Attrib_color);
	//glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_TRUE, 7 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(Attrib_vertex);
	//glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_TRUE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(Attrib_color);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cube3Tex::InitVO() {
	//InitVBO1();
	//InitVAO1();

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	//vartices = vertices;
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load and create a texture 
	// -------------------------
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("shaders/task3/ccad.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("shaders/task3/lena.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// or GL_RGBA if picture has alpha channel
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	glUseProgram(Program);
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(Program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(Program, "texture2"), 1);
}

void Cube3Tex::ReleaseVO() {
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glDeleteBuffers(1, &VBO1);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Cube3Tex::Draw() {
	glUseProgram(Program);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glUniform2f(glGetUniformLocation(Program, "offset"), offset[0], offset[1]);
	glUniform1f(glGetUniformLocation(Program, "mixRatio"), mixRatio);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(0);
	//checkOpenGLerror();
}