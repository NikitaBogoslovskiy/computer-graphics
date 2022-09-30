#ifndef _FUNCS_H_
#define _FUNCS_H

class Primitive
{
public:
	Primitive();
	virtual void draw();
};

class Point : public Primitive
{
public:
	int x, y;
	Point();
	Point(const float& x, const float& y);
};

class Edge : public Primitive
{
public:
	Point start, end;
	Edge();
	Edge(const float& x0, const float& y0, const float& x1, const float& y1);
};

#endif
