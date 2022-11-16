#include "../headers/geometry/primitives3d/rotation_body.h"

void RotationBody::performRotation(Primitive* prim, int iterNumber, Axis ax)
{
	unsigned int pointsNumber = prim->size();
	float alpha = -1;
	Eigen::MatrixXf pointsMatrix;
	Eigen::Matrix<float, 4, 4> rmat;
	float angle = 2 * PI / iterNumber;
	switch (ax)
	{
	case Axis::X:
	{
		float minX = FLT_MAX, maxX = FLT_MIN;
		for (auto v : *prim->getPoints())
		{
			if (v.x < minX)
				minX = v.x;
			if (v.x > maxX)
				maxX = v.x;
		}
		float betaX = -(minX + maxX) / 2;
		rmat = prepareDataX(prim, angle, alpha, betaX, pointsMatrix);
		break;
	}
	case Axis::Y:
	{
		float minY = FLT_MAX, maxY = FLT_MIN;
		for (auto v : *prim->getPoints())
		{
			if (v.y < minY)
				minY = v.y;
			if (v.y > maxY)
				maxY = v.y;
		}
		float betaY = -(minY + maxY) / 2;
		rmat = prepareDataY(prim, angle, alpha, betaY, pointsMatrix);
		break;
	}
	case Axis::Z:
	{
		rmat = prepareDataZ(prim, angle, alpha, NULL, pointsMatrix);
		break;
	}
	}
	for (unsigned int i = 0; i < iterNumber; ++i)
	{
		for (unsigned int j = 0; j < pointsNumber; ++j) {
			add_point(ImVec3(pointsMatrix(0, j), pointsMatrix(1, j), pointsMatrix(2, j)));
			if (j != pointsNumber - 1)
				add_polygon(Polygon{ j + i * pointsNumber, j + 1 + i * pointsNumber, j + 1 + ((i + 1) % iterNumber) * pointsNumber, j + ((i + 1) % iterNumber) * pointsNumber });
		}
		if (i != iterNumber - 1)
			pointsMatrix = rmat * pointsMatrix;
	}
	updateRTranslate();
}

Eigen::Matrix<float, 4, 4> RotationBody::prepareDataX(Primitive* prim, float angle, float alpha, float beta, Eigen::MatrixXf& pointsMatrix)
{
	Eigen::Matrix<float, 4, 4> rmat{
		{1, 0, 0, 0},
		{0, cos(-angle), sin(-angle), 0},
		{0, -sin(-angle), cos(-angle), 0},
		{0, 0, 0, 1}
	};
	pointsMatrix.resize(4, prim->size());
	for (size_t i = 0; i < prim->size(); i++) {
		pointsMatrix(0, i) = prim->at(i).x + beta;
		pointsMatrix(1, i) = alpha * prim->at(i).y;
		pointsMatrix(2, i) = 0;
		pointsMatrix(3, i) = 1;
	}
	return rmat;
}

Eigen::Matrix<float, 4, 4> RotationBody::prepareDataY(Primitive* prim, float angle, float alpha, float beta, Eigen::MatrixXf& pointsMatrix)
{
	Eigen::Matrix<float, 4, 4> rmat{
		{cos(-angle), 0, -sin(-angle), 0},
		{0, 1, 0, 0},
		{sin(-angle), 0, cos(-angle), 0},
		{0, 0, 0, 1}
	};
	pointsMatrix.resize(4, prim->size());
	for (size_t i = 0; i < prim->size(); i++) {
		pointsMatrix(0, i) = prim->at(i).x;
		pointsMatrix(1, i) = alpha * (prim->at(i).y + beta);
		pointsMatrix(2, i) = 0;
		pointsMatrix(3, i) = 1;
	}
	return rmat;
}

Eigen::Matrix<float, 4, 4> RotationBody::prepareDataZ(Primitive* prim, float angle, float alpha, float beta, Eigen::MatrixXf& pointsMatrix)
{
	Eigen::Matrix<float, 4, 4> rmat{
		{cos(-angle), sin(-angle), 0, 0},
		{-sin(-angle), cos(-angle), 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};
	pointsMatrix.resize(4, prim->size());
	for (size_t i = 0; i < prim->size(); i++) {
		pointsMatrix(0, i) = prim->at(i).x;
		pointsMatrix(1, i) = alpha * prim->at(i).y;
		pointsMatrix(2, i) = 0;
		pointsMatrix(3, i) = 1;
	}
	return rmat;
}
