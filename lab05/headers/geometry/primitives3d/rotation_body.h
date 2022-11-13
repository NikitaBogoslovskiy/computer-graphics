#pragma once

#include "mesh.h"

enum class Axis {X, Y, Z};

class RotationBody : public Mesh
{
public:
	RotationBody() {}
	RotationBody(Primitive* prim, int iterNumber, Axis ax) {
		performRotation(prim, iterNumber, ax);
	}
	void performRotation(Primitive* prim, int iterNumber, Axis ax);
private:
	Eigen::Matrix<float, 4, 4> prepareDataX(Primitive* prim, float angle, float alpha, float beta, Eigen::MatrixXf& pointsMatrix);
	Eigen::Matrix<float, 4, 4> prepareDataY(Primitive* prim, float angle, float alpha, float beta, Eigen::MatrixXf& pointsMatrix);
	Eigen::Matrix<float, 4, 4> prepareDataZ(Primitive* prim, float angle, float alpha, float beta, Eigen::MatrixXf& pointsMatrix);
};
