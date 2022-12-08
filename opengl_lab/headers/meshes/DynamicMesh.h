#pragma once

#include "./Mesh.h"

class DynamicMesh : public Mesh {
protected:
	virtual void InitShader() override;
public:
	DynamicMesh();
	DynamicMesh(const char* obj_path);
	virtual void UpdateUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) override;
};