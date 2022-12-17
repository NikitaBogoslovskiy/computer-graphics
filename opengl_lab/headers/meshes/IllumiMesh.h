#pragma once
#ifndef ILLUMI_MESH_H
#define ILLUMI_MESH_H

#include "Mesh.h"
#include "../lights/LightCasters.h"

class IllumiMesh : public Mesh {
	Material material;
	const PLS* pls;
	struct TransformLoc {
		GLuint model;
		GLuint view;
		GLuint projection;
		GLuint viewPos;
	} trLoc;

	struct MaterialLoc {
		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
		GLuint emission;
		GLuint shininess;
	} matLoc;

	struct PLSLoc {
		GLuint position;
		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
		GLuint attenuation;
	} plsLoc;
public:
	IllumiMesh();
	IllumiMesh(const char* obj_path);
	virtual void UpdateUniforms(const glm::mat4& model, Camera& cam) override;
	virtual void InitShader() override;
	virtual void ChangeShaders(const char* vertex_path, const char* fragment_path) override;
	void SetPLS(const PLS* const pls);
};

#endif //!ILLUMI_MESH_H