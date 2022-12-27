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
#ifdef DEBUG 
	printf("\033[0;33m====  IllumiMesh::ChangeShaders ====\033[0m\n");
#endif

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

	// spot light1 =================================================================================

	spsLoc1.position = glGetUniformLocation(Program, "sps1.position");
	spsLoc1.direction = glGetUniformLocation(Program, "sps1.direction");
	spsLoc1.eps = glGetUniformLocation(Program, "sps1.eps");
	spsLoc1.outerCutOff = glGetUniformLocation(Program, "sps1.outerCutOff");
	spsLoc1.ambient = glGetUniformLocation(Program, "sps1.ambient");
	spsLoc1.diffuse = glGetUniformLocation(Program, "sps1.diffuse");
	spsLoc1.specular = glGetUniformLocation(Program, "sps1.specular");
	spsLoc1.attenuation = glGetUniformLocation(Program, "sps1.attenuation");
	spsLoc1.intensity = glGetUniformLocation(Program, "sps1.intensity");

	if (spsLoc1.position == -1) printf("\033[0;31mcould not bind attrib sps.position\033[0m\n");
	if (spsLoc1.direction == -1) printf("\033[0;31mcould not bind attrib sps.direction\033[0m\n");
	if (spsLoc1.eps == -1) printf("\033[0;31mcould not bind attrib sps.eps\033[0m\n");
	if (spsLoc1.outerCutOff == -1) printf("\033[0;31mcould not bind attrib sps.outerCutOff\033[0m\n");

	if (spsLoc1.ambient == -1) printf("\033[0;31mcould not bind attrib sps.ambient\033[0m\n");
	if (spsLoc1.diffuse == -1) printf("\033[0;31mcould not bind attrib sps.diffuse\033[0m\n");
	if (spsLoc1.specular == -1) printf("\033[0;31mcould not bind attrib sps.specular\033[0m\n");
	if (spsLoc1.attenuation == -1) printf("\033[0;31mcould not bind attrib sps.attenuation\033[0m\n");
	if (spsLoc1.intensity == -1) printf("\033[0;31mcould not bind attrib sps.intensity\033[0m\n");

	// spot light2 =================================================================================

	spsLoc2.position = glGetUniformLocation(Program, "sps2.position");
	spsLoc2.direction = glGetUniformLocation(Program, "sps2.direction");
	spsLoc2.eps = glGetUniformLocation(Program, "sps2.eps");
	spsLoc2.outerCutOff = glGetUniformLocation(Program, "sps2.outerCutOff");
	spsLoc2.ambient = glGetUniformLocation(Program, "sps2.ambient");
	spsLoc2.diffuse = glGetUniformLocation(Program, "sps2.diffuse");
	spsLoc2.specular = glGetUniformLocation(Program, "sps2.specular");
	spsLoc2.attenuation = glGetUniformLocation(Program, "sps2.attenuation");
	spsLoc2.intensity = glGetUniformLocation(Program, "sps2.intensity");

	if (spsLoc2.position == -1) printf("\033[0;31mcould not bind attrib sps.position\033[0m\n");
	if (spsLoc2.direction == -1) printf("\033[0;31mcould not bind attrib sps.direction\033[0m\n");
	if (spsLoc2.eps == -1) printf("\033[0;31mcould not bind attrib sps.eps\033[0m\n");
	if (spsLoc2.outerCutOff == -1) printf("\033[0;31mcould not bind attrib sps.outerCutOff\033[0m\n");

	if (spsLoc2.ambient == -1) printf("\033[0;31mcould not bind attrib sps.ambient\033[0m\n");
	if (spsLoc2.diffuse == -1) printf("\033[0;31mcould not bind attrib sps.diffuse\033[0m\n");
	if (spsLoc2.specular == -1) printf("\033[0;31mcould not bind attrib sps.specular\033[0m\n");
	if (spsLoc2.attenuation == -1) printf("\033[0;31mcould not bind attrib sps.attenuation\033[0m\n");
	if (spsLoc2.intensity == -1) printf("\033[0;31mcould not bind attrib sps.intensity\033[0m\n");

#ifdef DEBUG 
	// ретурны бы дописать
	printf("\033[0;32m====  IllumiMesh::ChangeShaders success ====\033[0m\n");
#endif

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

	if (spotLight1 != nullptr) {
		glUniform3fv(spsLoc1.position, 1, glm::value_ptr(spotLight1->position));
		glUniform3fv(spsLoc1.direction, 1, glm::value_ptr(spotLight1->direction));
		glUniform3fv(spsLoc1.attenuation, 1, glm::value_ptr(spotLight1->attenuation));

		glUniform1f(spsLoc1.eps, 1.f / (spotLight1->cutOff - spotLight1->outerCutOff));
		glUniform1f(spsLoc1.outerCutOff, spotLight1->outerCutOff);

		glUniform4fv(spsLoc1.ambient, 1, glm::value_ptr(spotLight1->ambient));
		glUniform4fv(spsLoc1.diffuse, 1, glm::value_ptr(spotLight1->diffuse));
		glUniform4fv(spsLoc1.specular, 1, glm::value_ptr(spotLight1->specular));

		glUniform1f(spsLoc1.intensity, spotLight1->intensity);
	}

	if (spotLight2 != nullptr) {
		glUniform3fv(spsLoc2.position, 1, glm::value_ptr(spotLight2->position));
		glUniform3fv(spsLoc2.direction, 1, glm::value_ptr(spotLight2->direction));
		glUniform3fv(spsLoc2.attenuation, 1, glm::value_ptr(spotLight2->attenuation));

		glUniform1f(spsLoc2.eps, 1.f / (spotLight2->cutOff - spotLight2->outerCutOff));
		glUniform1f(spsLoc2.outerCutOff, spotLight2->outerCutOff);

		glUniform4fv(spsLoc2.ambient, 1, glm::value_ptr(spotLight2->ambient));
		glUniform4fv(spsLoc2.diffuse, 1, glm::value_ptr(spotLight2->diffuse));
		glUniform4fv(spsLoc2.specular, 1, glm::value_ptr(spotLight2->specular));

		glUniform1f(spsLoc2.intensity, spotLight2->intensity);
	}
}

void IllumiMesh::SetPLS(const PLS* const _pls)
{
	pls = _pls;
}

void IllumiMesh::SetSpotLights(const SpotLight* const _spotLight1, const SpotLight* const _spotLight2)
{
	spotLight1 = _spotLight1;
	spotLight2 = _spotLight2;
}

void IllumiMesh::SetDirLight(const DirLight* const _dirLight)
{
	dirLight = _dirLight;
}