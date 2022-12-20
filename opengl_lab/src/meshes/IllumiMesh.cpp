#include "../headers/meshes/IllumiMesh.h"

void IllumiMesh::InitShader() {
	ChangeShaders("shaders/mesh/default_l.vert", "shaders/mesh/default_l.frag");
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

void IllumiMesh::ChangeShaders(const char* vertex_path, const char* fragment_path)
{
	printf("\033[0;33m====  IllumiMesh::ChangeShaders ====\033[0m\n");
	Mesh::ChangeShaders(vertex_path, fragment_path);
	// retrieve the matrix uniform locations
	trLoc.model = glGetUniformLocation(Program, "transform.model");
	trLoc.view = glGetUniformLocation(Program, "transform.view");
	trLoc.projection = glGetUniformLocation(Program, "transform.projection");
	trLoc.normalTr = glGetUniformLocation(Program, "transform.normalTr");
	trLoc.viewPos = glGetUniformLocation(Program, "transform.viewPos");

	if (trLoc.model == -1) printf("\033[0;31mcould not bind attrib model\033[0m\n");
	if (trLoc.view == -1) printf("\033[0;31mcould not bind attrib view\033[0m\n");
	if (trLoc.projection == -1) printf("\033[0;31mcould not bind attrib projection\033[0m\n");
	if (trLoc.normalTr == -1) printf("\033[0;31mcould not bind attrib normalTr\033[0m\n");
	if (trLoc.viewPos == -1) printf("\033[0;31mcould not bind attrib viewPos\033[0m\n");

	matLoc.ambient = glGetUniformLocation(Program, "mtl.ambient");
	matLoc.diffuse = glGetUniformLocation(Program, "mtl.diffuse");
	matLoc.specular = glGetUniformLocation(Program, "mtl.specular");
	matLoc.emission = glGetUniformLocation(Program, "mtl.emission");
	matLoc.shininess = glGetUniformLocation(Program, "mtl.shininess");
	matLoc.roughness = glGetUniformLocation(Program, "mtl.roughness");
	matLoc.reflectivity = glGetUniformLocation(Program, "mtl.reflectivity");

	if (matLoc.ambient == -1) printf("\033[0;31mcould not bind attrib mtl.ambient\033[0m\n");
	if (matLoc.diffuse == -1) printf("\033[0;31mcould not bind attrib mtl.diffuse\033[0m\n");
	if (matLoc.specular == -1) printf("\033[0;31mcould not bind attrib mtl.specular\033[0m\n");
	if (matLoc.emission == -1) printf("\033[0;31mcould not bind attrib mtl.emission\033[0m\n");
	if (matLoc.shininess == -1) printf("\033[0;31mcould not bind attrib mtl.shininess\033[0m\n");
	if (matLoc.roughness == -1) printf("\033[0;31mcould not bind attrib mtl.roughness\033[0m\n");
	if (matLoc.reflectivity == -1) printf("\033[0;31mcould not bind attrib mtl.reflectivity\033[0m\n");

	// point light ================================================================================= 

	plsLoc.position = glGetUniformLocation(Program, "pls.position");
	plsLoc.ambient = glGetUniformLocation(Program, "pls.ambient");
	plsLoc.diffuse = glGetUniformLocation(Program, "pls.diffuse");
	plsLoc.specular = glGetUniformLocation(Program, "pls.specular");
	plsLoc.attenuation = glGetUniformLocation(Program, "pls.attenuation");
	plsLoc.intensity = glGetUniformLocation(Program, "pls.intensity");

	if (plsLoc.position == -1) printf("\033[0;31mcould not bind attrib pls.position\033[0m\n");
	if (plsLoc.ambient == -1) printf("\033[0;31mcould not bind attrib pls.ambient\033[0m\n");
	if (plsLoc.diffuse == -1) printf("\033[0;31mcould not bind attrib pls.diffuse\033[0m\n");
	if (plsLoc.specular == -1) printf("\033[0;31mcould not bind attrib pls.specular\033[0m\n");
	if (plsLoc.attenuation == -1) printf("\033[0;31mcould not bind attrib pls.attenuation\033[0m\n");
	if (plsLoc.intensity == -1) printf("\033[0;31mcould not bind attrib pls.intensity\033[0m\n");

	// directional light =================================================================================

	dlsLoc.direction = glGetUniformLocation(Program, "dls.direction");
	dlsLoc.ambient = glGetUniformLocation(Program, "dls.ambient");
	dlsLoc.diffuse = glGetUniformLocation(Program, "dls.diffuse");
	dlsLoc.specular = glGetUniformLocation(Program, "dls.specular");
	dlsLoc.intensity = glGetUniformLocation(Program, "dls.intensity");

	if (dlsLoc.direction == -1) printf("\033[0;31mcould not bind attrib dls.direction\033[0m\n");
	if (dlsLoc.ambient == -1) printf("\033[0;31mcould not bind attrib dls.ambient\033[0m\n");
	if (dlsLoc.diffuse == -1) printf("\033[0;31mcould not bind attrib dls.diffuse\033[0m\n");
	if (dlsLoc.specular == -1) printf("\033[0;31mcould not bind attrib dls.specular\033[0m\n");
	if (dlsLoc.intensity == -1) printf("\033[0;31mcould not bind attrib dls.intensity\033[0m\n");

	// spot light =================================================================================

	spsLoc.position = glGetUniformLocation(Program, "sps.position");
	spsLoc.direction = glGetUniformLocation(Program, "sps.direction");
	spsLoc.cutOff = glGetUniformLocation(Program, "sps.cutOff");
	spsLoc.outerCutOff = glGetUniformLocation(Program, "sps.outerCutOff");
	spsLoc.ambient = glGetUniformLocation(Program, "sps.ambient");
	spsLoc.diffuse = glGetUniformLocation(Program, "sps.diffuse");
	spsLoc.specular = glGetUniformLocation(Program, "sps.specular");
	spsLoc.attenuation = glGetUniformLocation(Program, "sps.attenuation");
	spsLoc.intensity = glGetUniformLocation(Program, "sps.intensity");

	if (spsLoc.position == -1) printf("\033[0;31mcould not bind attrib sps.position\033[0m\n");
	if (spsLoc.direction == -1) printf("\033[0;31mcould not bind attrib sps.direction\033[0m\n");
	if (spsLoc.cutOff == -1) printf("\033[0;31mcould not bind attrib sps.cutOff\033[0m\n");
	if (spsLoc.outerCutOff == -1) printf("\033[0;31mcould not bind attrib sps.outerCutOff\033[0m\n");

	if (spsLoc.ambient == -1) printf("\033[0;31mcould not bind attrib sps.ambient\033[0m\n");
	if (spsLoc.diffuse == -1) printf("\033[0;31mcould not bind attrib sps.diffuse\033[0m\n");
	if (spsLoc.specular == -1) printf("\033[0;31mcould not bind attrib sps.specular\033[0m\n");
	if (spsLoc.attenuation == -1) printf("\033[0;31mcould not bind attrib sps.attenuation\033[0m\n");
	if (spsLoc.intensity == -1) printf("\033[0;31mcould not bind attrib sps.intensity\033[0m\n");

	// ретурны бы дописать
	printf("\033[0;32m====  IllumiMesh::ChangeShaders success ====\033[0m\n");
}

void IllumiMesh::UpdateUniforms(const glm::mat4& model, Camera& cam)
{
	//printf("\033[0;33m=>  IllumiMesh::UpdateUniforms =\033[0m\n");
	auto view = cam.GetViewMatrix();
	auto proj = cam.GetProjectionMatrix();
	auto normalTr = glm::transpose(glm::inverse(glm::mat3(model)));
	Mesh::UpdateUniforms(model, cam);

	glUniformMatrix4fv(trLoc.model, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(trLoc.view, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(trLoc.projection, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix3fv(trLoc.normalTr, 1, GL_FALSE, glm::value_ptr(normalTr));
	glUniform3fv(trLoc.viewPos, 1, glm::value_ptr(cam.GetPosition()));

	glUniform4fv(matLoc.ambient, 1, glm::value_ptr(material.ambient));
	glUniform4fv(matLoc.diffuse, 1, glm::value_ptr(material.diffuse));
	glUniform4fv(matLoc.specular, 1, glm::value_ptr(material.specular));
	glUniform4fv(matLoc.emission, 1, glm::value_ptr(material.emission));
	glUniform1f(matLoc.shininess, material.shininess);
	glUniform1f(matLoc.roughness, material.roughness);
	glUniform1f(matLoc.reflectivity, material.reflectivity);

	if (pls != nullptr) {
		glUniform4fv(plsLoc.position, 1, glm::value_ptr(pls->position));
		glUniform4fv(plsLoc.ambient, 1, glm::value_ptr(pls->ambient));
		glUniform4fv(plsLoc.diffuse, 1, glm::value_ptr(pls->diffuse));
		glUniform4fv(plsLoc.specular, 1, glm::value_ptr(pls->specular));
		glUniform3fv(plsLoc.attenuation, 1, glm::value_ptr(pls->attenuation));

		glUniform1f(plsLoc.intensity, pls->intensity);
	}

	if (dirLight != nullptr) {
		glUniform3fv(dlsLoc.direction, 1, glm::value_ptr(dirLight->direction));
		glUniform4fv(dlsLoc.ambient, 1, glm::value_ptr(dirLight->ambient));
		glUniform4fv(dlsLoc.diffuse, 1, glm::value_ptr(dirLight->diffuse));
		glUniform4fv(dlsLoc.specular, 1, glm::value_ptr(dirLight->specular));

		glUniform1f(dlsLoc.intensity, dirLight->intensity);
	}

	if (spotLight != nullptr) {
		glUniform3fv(spsLoc.position, 1, glm::value_ptr(spotLight->position));
		glUniform3fv(spsLoc.direction, 1, glm::value_ptr(spotLight->direction));
		glUniform3fv(spsLoc.attenuation, 1, glm::value_ptr(spotLight->attenuation));

		glUniform1f(spsLoc.cutOff, spotLight->cutOff);
		glUniform1f(spsLoc.outerCutOff, spotLight->outerCutOff);

		glUniform4fv(spsLoc.ambient, 1, glm::value_ptr(spotLight->ambient));
		glUniform4fv(spsLoc.diffuse, 1, glm::value_ptr(spotLight->diffuse));
		glUniform4fv(spsLoc.specular, 1, glm::value_ptr(spotLight->specular));

		glUniform1f(spsLoc.intensity, spotLight->intensity);
	}
}

void IllumiMesh::SetPLS(const PLS* const _pls)
{
	pls = _pls;
}

void IllumiMesh::SetSpotLight(const SpotLight* const _spotLight)
{
	spotLight = _spotLight;
}

void IllumiMesh::SetDirLight(const DirLight* const _dirLight)
{
	dirLight = _dirLight;
}