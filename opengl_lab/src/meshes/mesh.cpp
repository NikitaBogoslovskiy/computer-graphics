#include "../headers/meshes/mesh.h"

void Mesh::InitVBO()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertex) * mVs.size(), mVs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint*) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

#include "../headers/shader_loader.h"

void Mesh::InitShader()
{
	Program = ShaderLoader::initProgram("shaders/mesh/default_s.vert", "shaders/mesh/default_s.frag");

	const char* attr_name = "coord"; //��� � �������
	Attrib_vertex = glGetAttribLocation(Program, attr_name);
	if (Attrib_vertex == -1) {
		printf("could not bind attrib %s\n", attr_name);
		return;
	}

	const char* attr_name2 = "texCoord"; //��� � �������
	Attrib_texture = glGetAttribLocation(Program, attr_name2);
	if (Attrib_texture == -1) {
		printf("could not bind attrib %s\n", attr_name2);
		return;
	}
	/*
	const char* attr_name3 = "normal";
	Attrib_normal = glGetAttribLocation(Program, attr_name3);
	if (Attrib_normal == -1) {
		printf("could not bind attrib %s\n", attr_name3);
		return;
	}
	*/
	//checkOpenGLerror();
}

#include "../headers/pch.h"

void Mesh::InitVO()
{
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	InitVBO();
	glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(Attrib_vertex);
	glVertexAttribPointer(Attrib_texture, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(Attrib_texture);
	//glVertexAttribPointer(Attrib_normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(Attrib_normal);

	glUniform1i(glGetUniformLocation(Program, "texture0"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ReleaseVO()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VAO);
}

#include "stb_image.h"

void Mesh::InitTextures()
{
	textures.push_back(0);

	glGenTextures(1, &textures.back());
	glBindTexture(GL_TEXTURE_2D, textures.back());
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis. but ive 
	unsigned char* data = stbi_load("shaders/task3/lena.jpg", &width, &height, &nrChannels, 0);
	if (*data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);
}

Mesh::Mesh()
{
	InitShader();
}

Mesh::Mesh(const char* path)
{
	Load(path);
	InitTextures();
	InitShader();
	InitVO();
}

void Mesh::ChangeShaders(const char* vertex_path, const char* fragment_path)
{
	Program = ShaderLoader::initProgram(vertex_path, fragment_path);

	const char* attr_name = "coord"; //��� � �������
	Attrib_vertex = glGetAttribLocation(Program, attr_name);
	if (Attrib_vertex == -1) {
		printf("could not bind attrib %s\n", attr_name);
		return;
	}

	const char* attr_name2 = "texCoord"; //��� � �������
	Attrib_texture = glGetAttribLocation(Program, attr_name2);
	if (Attrib_texture == -1) {
		printf("could not bind attrib %s\n", attr_name2);
		return;
	}
	/*
	const char* attr_name3 = "normal";
	Attrib_normal = glGetAttribLocation(Program, attr_name3);
	if (Attrib_normal == -1) {
		printf("could not bind attrib %s\n", attr_name3);
		return;
	}
	*/
}

void Mesh::Draw(const float& time)
{
	glUseProgram(Program);

	UpdateUniforms(time);

	glBindVertexArray(VAO);
	glMultiDrawElements(GL_TRIANGLE_FAN, count.data(), GL_UNSIGNED_INT, void_indices.data(), count.size());
	/* the same
	int x = 0;
	for (auto t : count) {
		glDrawElements(GL_TRIANGLE_FAN, t, GL_UNSIGNED_INT, (const void*)(x * sizeof(GLuint)));
		x += t;
	}
	*/
	
	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::UpdateUniforms(const float& time)
{
	uint i = 0;
	for (auto t : textures) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, t);
	}
}

#include <fstream>
#include <sstream>
#include <string>
#include <deque>

void Mesh::Load(const char* path)
{
	std::deque<std::pair<coord, std::deque<GLuint>>> v; // vertices
	std::deque<vertex_texture> vt; // textures
	std::deque<normal> vn; // normals

	std::ifstream infile(path);
	if (!infile.is_open()) {
		printf("path is wrong\n");
		return;
	}

	std::string line;
	char slash;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);

		std::string type;
		if (iss >> type, infile.eof()) {
			break;
		}

		if (type == "v") {
			coord _v;
			if (!(iss >> _v.x >> _v.y >> _v.z)) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			v.push_back({ _v, std::deque<GLuint>()});
		}
		else if (type == "vt") {
			vertex_texture _vt;
			if (!(iss >> _vt.u >> _vt.v)) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			vt.push_back(_vt);
		}
		else if (type == "vn") {
			coord _vn;
			if (!(iss >> _vn.x >> _vn.y >> _vn.z)) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			vn.push_back(_vn);
		}
		else if (type == "f") {
			p3ui32 inds;
			std::string buf;
			count.push_back(0);
			while (iss >> buf) {
				std::istringstream iss2(buf);
				iss2 >> inds.v() >> slash >> inds.vt() >> slash >> inds.vn();
				bool do_create_new = false;
				if (v[inds.v() - 1].second.empty()) do_create_new = true;
				for (auto _mv : v[inds.v() - 1].second) {
					// check vt
					// if not equal create new mV
					if (vt[inds.vt() - 1] != mVs[_mv].vt) {
						do_create_new = true;
						break;
					}
					// check vn
					// if not equal create new mV
					if (vn[inds.vn() - 1] != mVs[_mv].vn) {
						do_create_new = true;
						break;
					}
					indices.push_back(_mv);
					break;
				}
				if (do_create_new) {
					mVs.push_back({ 
						v[inds.v() - 1].first, 
						vt[inds.vt() - 1], 
						vn[inds.vn() - 1] });
					v[inds.v() - 1].second.push_back(mVs.size() - 1);
					indices.push_back(mVs.size() - 1);
				}
				count.back()++;
			}

			if (count.back() < 3) {
				printf("File can't be read by our simple parser\n");
				return;
			}
		}
	}
	infile.close();

	GLuint x = 0;
	for (GLuint t : count)
	{
		void_indices.push_back((GLvoid*)(indices[x] * sizeof(GLuint)));
		x += t;
	}
}

void Mesh::LoadTexture(const char* path, uint texturei)
{
	if (textures.size() <= texturei) {
		printf("undefined texture number\n");
		return;
	}
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis. but ive 
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (*data)
	{
		glBindTexture(GL_TEXTURE_2D, textures[texturei]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);
}

void Mesh::AddTexture(const char* path)
{
	if (textures.size() == 32) {
		printf("Textures are full\n");
		return;
	}

	textures.push_back(0);
	glGenTextures(1, &textures.back());

	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis. but ive 
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (*data)
	{
		glBindTexture(GL_TEXTURE_2D, textures.back());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		textures.pop_back();
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);
}

Mesh::~Mesh()
{
	Release();
}

DynamicMesh::DynamicMesh()
{
	InitShader();
}

DynamicMesh::DynamicMesh(const char* path)
{
	Load(path);
	InitTextures();
	InitShader();
	InitVO();
}

void DynamicMesh::InitShader()
{
	ChangeShaders("shaders/mesh/default_d.vert", "shaders/mesh/default_d.frag");
}

void DynamicMesh::UpdateUniforms(const float& time)
{
	Mesh::UpdateUniforms(time);
	model = glm::rotate(model, glm::radians(time * 30.f), glm::vec3(0.5f, 1.0f, 0.f));
	view = glm::translate(view, glm::vec3(offset.x, offset.y, offset.z));

	// retrieve the matrix uniform locations
	unsigned int modelLoc = glGetUniformLocation(Program, "model");
	unsigned int viewLoc = glGetUniformLocation(Program, "view");
	unsigned int projectionLoc = glGetUniformLocation(Program, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	/*
	*/
}
