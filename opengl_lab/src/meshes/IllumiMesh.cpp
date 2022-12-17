#include "../headers/meshes/IllumiMesh.h"

void IllumiMesh::InitShader() {
	printf("== IllumiMesh::InitShader ==\n");
	ChangeShaders("shaders/mesh/default_l.vert", "shaders/mesh/default_l.frag");
	printf("== IllumiMesh::InitShader end ==\n");
}

void IllumiMesh::ChangeShaders(const char* vertex_path, const char* fragment_path)
{
	printf("\033[0;33m====  IllumiMesh::ChangeShaders ====\033[0m\n");
	Mesh::ChangeShaders(vertex_path, fragment_path);
	// retrieve the matrix uniform locations
	trLoc.model = glGetUniformLocation(Program, "model");
	trLoc.view = glGetUniformLocation(Program, "view");
	trLoc.projection = glGetUniformLocation(Program, "projection");
	trLoc.viewPos = glGetUniformLocation(Program, "viewPos");

	if (trLoc.model == -1) printf("\033[0;31mcould not bind attrib model\033[0m\n");
	if (trLoc.view == -1) printf("\033[0;31mcould not bind attrib view\033[0m\n");
	if (trLoc.projection == -1) printf("\033[0;31mcould not bind attrib projection\033[0m\n");
	if (trLoc.viewPos == -1) printf("\033[0;31mcould not bind attrib viewPos\033[0m\n");

	matLoc.ambient = glGetUniformLocation(Program, "mtl.ambient");
	matLoc.diffuse = glGetUniformLocation(Program, "mtl.diffuse");
	matLoc.specular = glGetUniformLocation(Program, "mtl.specular");
	matLoc.emission = glGetUniformLocation(Program, "mtl.emission");
	matLoc.shininess = glGetUniformLocation(Program, "mtl.shininess");

	if (matLoc.ambient == -1) printf("\033[0;31mcould not bind attrib mtl.ambient\033[0m\n");
	if (matLoc.diffuse == -1) printf("\033[0;31mcould not bind attrib mtl.diffuse\033[0m\n");
	if (matLoc.specular == -1) printf("\033[0;31mcould not bind attrib mtl.specular\033[0m\n");
	if (matLoc.emission == -1) printf("\033[0;31mcould not bind attrib mtl.emission\033[0m\n");
	if (matLoc.shininess == -1) printf("\033[0;31mcould not bind attrib mtl.shininess\033[0m\n");

	plsLoc.position = glGetUniformLocation(Program, "pls.position");
	plsLoc.ambient = glGetUniformLocation(Program, "pls.ambient");
	plsLoc.diffuse = glGetUniformLocation(Program, "pls.diffuse");
	plsLoc.specular = glGetUniformLocation(Program, "pls.specular");
	plsLoc.attenuation = glGetUniformLocation(Program, "pls.attenuation");

	if (plsLoc.position == -1) printf("\033[0;31mcould not bind attrib pls.position\033[0m\n");
	if (plsLoc.ambient == -1) printf("\033[0;31mcould not bind attrib pls.ambient\033[0m\n");
	if (plsLoc.diffuse == -1) printf("\033[0;31mcould not bind attrib pls.diffuse\033[0m\n");
	if (plsLoc.specular == -1) printf("\033[0;31mcould not bind attrib pls.specular\033[0m\n");
	if (plsLoc.attenuation == -1) printf("\033[0;31mcould not bind attrib pls.attenuation\033[0m\n");

	printf("\033[0;32m====  IllumiMesh::ChangeShaders success ====\033[0m\n");
}

void IllumiMesh::SetPLS(const PLS* const _pls)
{
	pls = _pls;
}

IllumiMesh::IllumiMesh()
{
	InitShader();
}

IllumiMesh::IllumiMesh(const char* obj_path)
{
	Load(obj_path);
	InitTextures();
	InitShader();
	InitVO();
}

void IllumiMesh::UpdateUniforms(const glm::mat4& model, Camera& cam)
{
	auto proj = cam.GetProjectionMatrix();
	Mesh::UpdateUniforms(model, cam);

	glUniformMatrix4fv(trLoc.model, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(trLoc.view, 1, GL_FALSE, glm::value_ptr(cam.GetViewMatrix()));
	glUniformMatrix4fv(trLoc.projection, 1, GL_FALSE, glm::value_ptr(proj));
	glUniform3fv(trLoc.viewPos, 1, glm::value_ptr(cam.GetPosition()));


	glUniform4fv(matLoc.ambient, 1, glm::value_ptr(material.ambient));
	glUniform4fv(matLoc.diffuse, 1, glm::value_ptr(material.diffuse));
	glUniform4fv(matLoc.specular, 1, glm::value_ptr(material.specular));
	glUniform4fv(matLoc.emission, 1, glm::value_ptr(material.emission));
	glUniform1f(matLoc.shininess, material.shininess);

	if (pls) {
		glUniform4fv(plsLoc.position, 1, glm::value_ptr(pls->position));
		glUniform4fv(plsLoc.ambient, 1, glm::value_ptr(pls->ambient));
		glUniform4fv(plsLoc.diffuse, 1, glm::value_ptr(pls->diffuse));
		glUniform4fv(plsLoc.specular, 1, glm::value_ptr(pls->specular));
		glUniform3fv(plsLoc.attenuation, 1, glm::value_ptr(pls->attenuation));
	}
	/*
	*/
}