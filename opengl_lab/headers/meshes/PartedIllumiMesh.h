#pragma once
#ifndef PARTEDILLUMIMESH_H
#define PARTEDILLUMIMESH_H

#include "IllumiMesh.h"

class PartedIllumiMesh : public IllumiMesh {
	// first.first - a number of the first face of a mesh
	// first.first - a number of the first vertex of a mesh
	// second - a number of a material
	std::vector <std::pair<std::pair<int, int>, int>> mesh_indecies;
	std::vector<Material> materials;
	// frequently changing value, dont use!
	int current_material = 0;
	void LoadMaterials(const char* mtllib_path, std::map<const char*, int>* mat_table);
public:
	PartedIllumiMesh();
	PartedIllumiMesh(const char* obj_path);
	virtual void Load(const char* path) override;
	virtual void UpdateUniforms(const glm::mat4& model, Camera& cam) override;
	virtual void Draw(const glm::mat4& model, Camera& cam) override;
};
#endif //!PARTEDILLUMIMESH_H
