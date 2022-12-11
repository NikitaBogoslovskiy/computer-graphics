#pragma once
#ifndef MESH_H
#define MESH_H

#include "../entities/Entity.h"
#include <vector>
#include "../stuff.h"

class Mesh : public Entity
{
protected:
	/*
		Guide: where mf my faces?
		We are using indices(offsets) and an array of fully complected vertexes,
		so our faces are realised like abstract objects via count.
		For example: count[0] says us about vertices in the first face, where indices 
		are linked with them in a local array buffer.
		My head fell apart when I did this, please dont look at the implementation 
		cause you might get a little crazy.
	*/

	GLuint Attrib_normal;
	//glm::mat4 model = glm::mat4(1.0f);
	//glm::mat4 view = glm::mat4(1.0f);

	void InitVBO(); // VBO, IBO
	virtual void InitShader() override;
	virtual void ReleaseVO() override; // VBO, IBO, VAO
	
	virtual void UpdateUniforms(const glm::mat4& model, Camera& cam);

	using vertexData = mVertex;
public:
	GLuint Attrib_texture;
	std::vector<GLuint> textures;
	std::vector<GLsizei> count; // for glMultiDrawElements 
	std::vector<vertexData> mVs; // vertices(full packed)
	std::vector<GLuint> indices; // for IBO
	std::vector<GLvoid*> void_indices; // for glMultiDrawElements
	Mesh(); 
	Mesh(const char* obj_path);

	void Load(const char* path);
	void InitTextures(char* path = (char*)"shaders/task3/lena.jpg");
	void LoadTexture(const char* path, uint texturei);
	void AddTexture(const char* path);
	virtual void ChangeShaders(const char* vertex_path, const char* fragment_path);
	void InitVO() override; // VAO

	//void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) final;
	void Draw(const glm::mat4& model, Camera& cam) final;

	~Mesh();
};

#endif // !MESH_H
