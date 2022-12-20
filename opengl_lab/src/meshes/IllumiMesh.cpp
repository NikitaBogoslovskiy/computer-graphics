#include "../headers/meshes/IllumiMesh.h"

void IllumiMesh::InitShader() {
	ChangeShaders("shaders/mesh/default_l.vert", "shaders/mesh/default_l.frag");
}

void IllumiMesh::ChangeShaders(const char* vertex_path, const char* fragment_path)
{
	Mesh::ChangeShaders(vertex_path, fragment_path);
	// retrieve the matrix uniform locations
	trLoc.model = glGetUniformLocation(Program, "model");
	trLoc.view = glGetUniformLocation(Program, "view");
	trLoc.projection = glGetUniformLocation(Program, "projection");
	trLoc.viewPos = glGetUniformLocation(Program, "viewPos");

	if (trLoc.model == -1) printf("could not bind attrib model\n");
	if (trLoc.view == -1) printf("could not bind attrib view\n");
	if (trLoc.projection == -1) printf("could not bind attrib projection\n");
	if (trLoc.viewPos == -1) printf("could not bind attrib viewPos\n");

	matLoc.ambient = glGetUniformLocation(Program, "ambient");
	matLoc.diffuse = glGetUniformLocation(Program, "diffuse");
	matLoc.specular = glGetUniformLocation(Program, "specular");
	matLoc.emission = glGetUniformLocation(Program, "emission");
	matLoc.shininess = glGetUniformLocation(Program, "shininess");

	if (matLoc.ambient == -1) printf("could not bind attrib ambient\n");
	if (matLoc.diffuse == -1) printf("could not bind attrib diffuse\n");
	if (matLoc.specular == -1) printf("could not bind attrib specular\n");
	if (matLoc.emission == -1) printf("could not bind attrib emission\n");
	if (matLoc.shininess == -1) printf("could not bind attrib shininess\n");

	plsLoc.position = glGetUniformLocation(Program, "pls_position");
	plsLoc.ambient = glGetUniformLocation(Program, "pls_ambient");
	plsLoc.diffuse = glGetUniformLocation(Program, "pls_diffuse");
	plsLoc.specular = glGetUniformLocation(Program, "pls_specular");
	plsLoc.attenuation = glGetUniformLocation(Program, "pls_attenuation");

	if (plsLoc.position == -1) printf("could not bind attrib pls_position\n");
	if (plsLoc.ambient == -1) printf("could not bind attrib pls_ambient\n");
	if (plsLoc.diffuse == -1) printf("could not bind attrib pls_diffuse\n");
	if (plsLoc.specular == -1) printf("could not bind attrib pls_specular\n");
	if (plsLoc.attenuation == -1) printf("could not bind attrib pls_attenuation\n");

}

void IllumiMesh::SetPLS(const PLS* const _pls)
{
	pls = _pls;
}

IllumiMesh::IllumiMesh()
{
	//InitShader(); NO MORE INIT SHADERS IN CONSTRUCTORS USING 1000000000000000000 WRAPPER CLASSES
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