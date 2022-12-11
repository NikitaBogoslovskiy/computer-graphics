#include "../headers/meshes/DynamicMesh.h"

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

void DynamicMesh::UpdateUniforms(const glm::mat4& model, Camera& cam)
{
	Mesh::UpdateUniforms(model, cam);

	// retrieve the matrix uniform locations
	unsigned int modelLoc = glGetUniformLocation(Program, "model");
	unsigned int viewLoc = glGetUniformLocation(Program, "view");
	unsigned int projectionLoc = glGetUniformLocation(Program, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.GetViewMatrix()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(cam.GetProjectionMatrix()));
	/*
	*/
}