#include "../headers/funcs.h"

Primitive::Primitive() {};
void Primitive::draw() {};

Point::Point()
{
	Point::Primitive();
}

Point::Point(const float& x, const float& y)
{
	Point::Primitive();
	this->x = x;
	this->y = y;
}

Edge::Edge()
{
	Edge::Primitive();
}

Edge::Edge(const float& x0, const float& y0, const float& x1, const float& y1)
{
	Edge::Primitive();
	this->start = Point(x0, y0);
	this->end = Point(x1, y1);
}

