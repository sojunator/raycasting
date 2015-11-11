#ifndef LABORATION_H
#define LABORATION_H
#include "Shapes.h"

class LPlane: public Shape
{
	Vec		n;
	float	d;
public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point) { return n; }
	LPlane(Vec normal, float _d, Color color);

};


class LSphere : public Shape
{
	Vec center;
	float radius;
	float radius2;
public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point);
	LSphere(Vec _center, float _radius, Color _color);
};


class LTriangle : public Shape
{
	Vec p1, p2, p3, nor, plane;
	Vec edge0, edge1;
	float d;
public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point) { return nor; }
	LTriangle(Vec _p1, Vec _p2, Vec _p3, Color _color);

};

float angle(Vec& v1, Vec& v2);
Vec cross(const Vec& v1, const Vec& v2);


#endif
