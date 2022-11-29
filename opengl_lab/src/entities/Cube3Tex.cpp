#include "../../headers/entities/Cube3Tex.h"
#include "stb_image.h"
Cube3Tex::Cube3Tex(): MixedCube() {
	InitShader();
	InitVO();
}

void Cube3Tex::InitShader() {
	Program = ShaderLoader::initProgram("shaders/task3/tt_cube.vert", "shaders/task3/tt_cube.frag");
	
	// no need to seek for attributes in shader cuz there are so called "locations". saves time a bit lol
	Attrib_vertex = 0;
	Attrib_texture = 1;

	//checkOpenGLerror();
}

void Cube3Tex::InitVO() {

	float vertices[] = {
		left_bottom_near, lb,
		right_bottom_near, rb,
		right_top_near, rt,
		right_top_near, rt,
		left_top_near, lt,
		left_bottom_near, lb,

		right_bottom_near, lb,
		right_bottom_far, rb,
		right_top_far, rt,
		right_top_far, rt,
		right_top_near, lt,
		right_bottom_near, lb,

		right_bottom_far, lb,
		left_bottom_far, rb,
		left_top_far, rt,
		left_top_far, rt,
		right_top_far, lt,
		right_bottom_far, lb,

		left_bottom_far, lb,
		left_bottom_near, rb,
		left_top_near, rt,
		left_top_near, rt,
		left_top_far, lt,
		left_bottom_far, lb,

		left_top_near, lb,
		right_top_near, rb,
		right_top_far, rt,
		right_top_far, rt,
		left_top_far, lt,
		left_top_near, lb,

		left_bottom_far, lb,
		right_bottom_far, rb,
		right_bottom_near, rt,
		right_bottom_near, rt,
		left_bottom_near, lt,
		left_bottom_far, lb,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// THE RIGHT ORDER IS bind the VAO first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(Attrib_vertex);
	// texture coord attribute
	glVertexAttribPointer(Attrib_texture, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(Attrib_texture);

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
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis. but ive 
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
	glUniform1i(glGetUniformLocation(Program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(Program, "texture2"), 1);
	glUseProgram(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cube3Tex::ReleaseVO() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Cube3Tex::Draw(const float& time) {
	glUseProgram(Program);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// create transformations
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(time * 30.f), glm::vec3(0.5f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, zOffset));
	projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	// retrieve the matrix uniform locations
	unsigned int modelLoc = glGetUniformLocation(Program, "model");
	unsigned int viewLoc = glGetUniformLocation(Program, "view");
	unsigned int projectionLoc = glGetUniformLocation(Program, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glUniform1f(glGetUniformLocation(Program, "mixRatio"), mixRatio);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glUseProgram(0);
	//checkOpenGLerror();
}