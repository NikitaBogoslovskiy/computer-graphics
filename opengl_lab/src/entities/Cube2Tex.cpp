#include "../../headers/entities/Cube2Tex.h"
#include "stb_image.h"
Cube2Tex::Cube2Tex() : MixedCube() {
	InitShader();
	InitVO();
}

void Cube2Tex::InitShader() {
	Program = ShaderLoader::initProgram("shaders/task2/nr_cube.vert", "shaders/task2/nr_cube.frag");

	// no need to seek for attributes in shader cuz there are so called "locations". saves time a bit lol
	Attrib_vertex = 0;
	Attrib_color = 1;
	Attrib_texture = 2;

	//checkOpenGLerror();
}

void Cube2Tex::InitVO() {
	float vertices[] = {
		left_bottom_near, red, lb,
		right_bottom_near, blue, rb,
		right_top_near, green, rt,
		right_top_near, green, rt,
		left_top_near, white, lt,
		left_bottom_near, red, lb,

		right_bottom_near, blue, lb,
		right_bottom_far, white, rb,
		right_top_far, red, rt,
		right_top_far, red, rt,
		right_top_near, green, lt,
		right_bottom_near, blue, lb,

		right_bottom_far, white, lb,
		left_bottom_far, green, rb,
		left_top_far, blue, rt,
		left_top_far, blue, rt,
		right_top_far, red, lt,
		right_bottom_far, white, lb,

		left_bottom_far, green, lb,
		left_bottom_near, red, rb,
		left_top_near,white, rt,
		left_top_near, white,rt,
		left_top_far, blue, lt,
		left_bottom_far, green, lb,

		left_top_near, white, lb,
		right_top_near, green, rb,
		right_top_far, red, rt,
		right_top_far, red, rt,
		left_top_far, blue, lt,
		left_top_near, white, lb,

		left_bottom_far, green, lb,
		right_bottom_far, white, rb,
		right_bottom_near, blue, rt,
		right_bottom_near, blue, rt,
		left_bottom_near, red, lt,
		left_bottom_far, green, lb,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// THE RIGHT ORDER IS bind the VAO first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(Attrib_vertex);
	// color attribute
	glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(Attrib_color);
	// texture coord attribute
	glVertexAttribPointer(Attrib_texture, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(Attrib_texture);

	// load and create a texture 
   // -------------------------
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("shaders/task2/kroshy.jpg", &width, &height, &nrChannels, 0);
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cube2Tex::ReleaseVO() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Cube2Tex::Draw(const float& time) {
	glUseProgram(Program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// create transformations
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 view = glm::mat4(1.0f);
	
	model = glm::rotate(model, glm::radians(time * 30.f), glm::vec3(0.5f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(offset.x, offset.y, offset.z));

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