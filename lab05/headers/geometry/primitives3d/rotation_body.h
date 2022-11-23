#pragma once

#include "mesh.h"

enum class Axis {X, Y, Z};

class RotationBody : public Mesh
{
public:
	RotationBody() {}
	RotationBody(Primitive* prim, int iterNumber, Axis ax) {
		performRotation(prim, iterNumber, ax);
		recalculate_normals();
		use_normals = false;
	}
	void performRotation(Primitive* prim, int iterNumber, Axis ax);
	virtual inline void add_polygon(const Polygon& polygon) override { polygons.push_back(polygon); }
	virtual inline void add_polygon(Polygon&& polygon) override { polygons.push_back(std::move(polygon)); }
private:
	Eigen::Matrix<float, 4, 4> prepareDataX(Primitive* prim, float angle, float alpha, float beta, Eigen::MatrixXf& pointsMatrix);
	Eigen::Matrix<float, 4, 4> prepareDataY(Primitive* prim, float angle, float alpha, float beta, Eigen::MatrixXf& pointsMatrix);
	Eigen::Matrix<float, 4, 4> prepareDataZ(Primitive* prim, float angle, float alpha, float beta, Eigen::MatrixXf& pointsMatrix);
};
