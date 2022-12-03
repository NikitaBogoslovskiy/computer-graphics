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
	std::vector<mVertex> mVs; // vertices(full packed)
	std::vector<GLsizei> count; // for glMultiDrawElements 
	std::vector<GLuint> indices; // for IBO
	std::vector<GLvoid*> void_indices; // for glMultiDrawElements

	GLuint Attrib_texture;
	GLuint Attrib_normal;

	std::vector<GLuint> textures;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	void InitVBO(); // VBO, IBO
	void InitShader() override;
	void InitVO() override; // VAO

	void ReleaseVO() override; // VBO, IBO, VAO

	void InitTextures();
	
	virtual void UpdateUniforms(const float& time);
public:

	Mesh(); 
	Mesh(const char* obj_path);

	void Load(const char* path);
	void LoadTexture(const char* path, uint texturei);
	void AddTexture(const char* path);
	void ChangeShaders(const char* vertex_path, const char* fragment_path);

	void Draw(const float& time) final;

	~Mesh();
};

class DynamicMesh : public Mesh {
protected:
	virtual void InitShader() override;
	virtual void UpdateUniforms(const float& time) override;
public:
	DynamicMesh();
	DynamicMesh(const char* obj_path);
};

#endif // !MESH_H
