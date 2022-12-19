#include "../headers/meshes/PartedIllumiMesh.h"
#include <deque>
#include <fstream>
#include <sstream>

void PartedIllumiMesh::LoadMaterials(const char* mtllib_path, std::map<const char*, int>* mat_table)
{
	mat_table->clear();
	materials.clear();
	materials.push_back(Material()); // default

	std::ifstream infile(mtllib_path);
	if (!infile.is_open()) {
		printf("file is not found\n");
		return;
	}

	std::string line;
	char slash;
	std::string temp;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);

		std::string type;
		if (iss >> type, infile.eof()) {
			break;
		}

		if (type == "newmtl") {
			(*mat_table)[(iss >> temp, temp.c_str())] = materials.size();
			materials.push_back(Material());
		}
		else if (type == "Ka") {
			glm::vec4 t;
			iss >> t.x >> t.y >> t.z;
			t.w = 1.f;
			materials.back().ambient = std::move(t);
		}
		else if (type == "Kd") {
			glm::vec4 t;
			iss >> t.x >> t.y >> t.z;
			t.w = 1.f;
			materials.back().diffuse = std::move(t);
		}
		else if (type == "Ks") {
			glm::vec4 t;
			iss >> t.x >> t.y >> t.z;
			t.w = 1.f;
			materials.back().specular = std::move(t);
		}
		else if (type == "Ns") {
			iss >> materials.back().shininess;
		}
		else if (type == "Ke") {
			glm::vec4 t;
			iss >> t.x >> t.y >> t.z;
			t.w = 1.f;
			materials.back().emission = std::move(t);
		}
		else if (type == "map_Kd") {
			// load texture
		}
	}

	infile.close();
}

PartedIllumiMesh::PartedIllumiMesh()
{
	InitShader();
}

PartedIllumiMesh::PartedIllumiMesh(const char* obj_path)
{
	Load(obj_path);
	InitTextures();
	InitShader();
	InitVO();
}

void PartedIllumiMesh::Load(const char* path)
{
	std::deque<std::pair<coord, std::deque<GLuint>>> v; // vertices
	std::deque<vertex_texture> vt; // textures
	std::deque<normal> vn; // normals
	std::map<const char*, int> materials_table;

	std::ifstream infile(path);
	if (!infile.is_open()) {
		printf("path is wrong\n");
		return;
	}

	std::string line;
	char slash;
	std::string temp;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);

		std::string type;
		if (iss >> type, infile.eof()) {
			break;
		}

		if (type == "v") {
			coord _v;
			if (!(iss >> _v.x >> _v.y >> _v.z)) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			v.push_back({ _v, std::deque<GLuint>() });
		}
		else if (type == "vt") {
			vertex_texture _vt;
			if (!(iss >> _vt.u >> _vt.v)) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			vt.push_back(_vt);
		}
		else if (type == "vn") {
			normal _vn;
			if (!(iss >> _vn.x >> _vn.y >> _vn.z)) {
				printf("File can't be read by our simple parser\n");
				return;
			}
			vn.push_back(_vn);
		}
		else if (type == "mtllib") {
			LoadMaterials((iss >> temp, temp.c_str()), &materials_table);
		}
		else if (type == "usemtl") {
			mesh_indecies.push_back({ {count.size(), mVs.size()}, materials_table[(iss >> temp, temp.c_str())]});
		}
		else if (type == "f") {
			if (mesh_indecies.empty()) {
				mesh_indecies.push_back({{0, 0}, 0});
				if (materials.empty()) {
					materials.push_back(Material());
				}
			}
			p3ui32 inds;
			std::string buf;
			count.push_back(0);
			while (iss >> buf) {
				std::istringstream iss2(buf);
				iss2 >> inds.v() >> slash >> inds.vt() >> slash >> inds.vn();
				bool do_create_new = false;
				if (v[inds.v() - 1].second.empty()) do_create_new = true;
				do_create_new = true;
				/*
				for (auto _mv : v[inds.v() - 1].second) {
					// check vt
					// if not equal create new mV

					if (vt[inds.vt() - 1] != mVs[_mv].vt) {
						do_create_new = true;
						break;
					}

					// check vn
					// if not equal create new mV

					if (vn[inds.vn() - 1] != mVs[_mv].vn) {
						do_create_new = true;
						break;
					}

					indices.push_back(_mv);
					break;
				}
				*/
				if (do_create_new) {
					mVs.push_back({
						v[inds.v() - 1].first,
						(inds.vt() ? vt[inds.vt() - 1] : vertex_texture{randf(), randf()}),
						(inds.vn() ? vn[inds.vn() - 1] : normal(randf(), randf(), randf()).normilize())
						});
					v[inds.v() - 1].second.push_back(mVs.size() - 1);
					indices.push_back(mVs.size() - 1);
				}
				count.back()++;
			}

			if (count.back() < 3) {
				printf("File can't be read by our simple parser\n");
				return;
			}
		}
	}
	infile.close();

	GLuint x = 0;
	for (GLuint t : count)
	{
		void_indices.push_back((GLvoid*)(indices[x] * sizeof(GLuint)));
		x += t;
	}
}

void PartedIllumiMesh::UpdateUniforms(const glm::mat4& model, Camera& cam)
{
	auto proj = cam.GetProjectionMatrix();
	Mesh::UpdateUniforms(model, cam);

	glUniformMatrix4fv(trLoc.model, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(trLoc.view, 1, GL_FALSE, glm::value_ptr(cam.GetViewMatrix()));
	glUniformMatrix4fv(trLoc.projection, 1, GL_FALSE, glm::value_ptr(proj));
	glUniform3fv(trLoc.viewPos, 1, glm::value_ptr(cam.GetPosition()));


	glUniform4fv(matLoc.ambient, 1, glm::value_ptr(materials[current_material].ambient));
	glUniform4fv(matLoc.diffuse, 1, glm::value_ptr(materials[current_material].diffuse));
	glUniform4fv(matLoc.specular, 1, glm::value_ptr(materials[current_material].specular));
	glUniform4fv(matLoc.emission, 1, glm::value_ptr(materials[current_material].emission));
	glUniform1f(matLoc.shininess, materials[current_material].shininess);

	if (pls) {
		glUniform4fv(plsLoc.position, 1, glm::value_ptr(pls->position));
		glUniform4fv(plsLoc.ambient, 1, glm::value_ptr(pls->ambient));
		glUniform4fv(plsLoc.diffuse, 1, glm::value_ptr(pls->diffuse));
		glUniform4fv(plsLoc.specular, 1, glm::value_ptr(pls->specular));
		glUniform3fv(plsLoc.attenuation, 1, glm::value_ptr(pls->attenuation));
	}
}

void PartedIllumiMesh::Draw(const glm::mat4& model, Camera& cam)
{
	glUseProgram(Program);
	this->UpdateUniforms(model, cam);

	glBindVertexArray(VAO);
	for (auto& pmi : mesh_indecies)
	{
		current_material = pmi.second;
		glMultiDrawElements(GL_TRIANGLE_FAN, count.data() + pmi.ff, GL_UNSIGNED_INT, void_indices.data() + pmi.fs, count.size() + pmi.ff);
	}	

	glBindVertexArray(0);
	glUseProgram(0);
}
