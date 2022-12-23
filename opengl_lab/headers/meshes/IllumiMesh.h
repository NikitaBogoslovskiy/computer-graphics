#pragma once
#ifndef ILLUMI_MESH_H
#define ILLUMI_MESH_H

#include "Mesh.h"
#include "../lights/LightCasters.h"

class IllumiMesh : public Mesh {
	Material material;
protected:
	const PLS* pls;
	const DirLight* dirLight;
	const SpotLight* spotLight;

	struct TransformLoc {
		GLuint model;
		GLuint view;
		GLuint projection;
		GLuint normalTr;
		GLuint viewPos;
	} trLoc;

	struct MaterialLoc {
		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
		GLuint emission;
		GLuint shininess;
		GLuint roughness;
		GLuint reflectivity;
	} matLoc;

	struct PLSLoc {
		GLuint position;
		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
		GLuint attenuation;

		GLuint intensity;
	} plsLoc;

	struct DirLoc {
		GLuint direction;
		GLuint ambient;
		GLuint diffuse;
		GLuint specular;

		GLuint intensity;
	} dlsLoc;

	struct SPLoc {
		GLuint position;
		GLuint direction;

		GLuint eps;
		GLuint outerCutOff;

		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
		GLuint attenuation;

		GLuint intensity;
	} spsLoc;
public:
	IllumiMesh();
	IllumiMesh(const char* obj_path);
	virtual void UpdateUniforms(const glm::mat4& model, Camera& cam) override;
	virtual void InitShader() override;
	virtual void ChangeShaders(const char* vertex_path, const char* fragment_path) override;
	void SetPLS(const PLS* const pls);
	void SetSpotLight(const SpotLight* const _spotLight);
	void SetDirLight(const DirLight* const _dirLight);
};

#endif //!ILLUMI_MESH_H