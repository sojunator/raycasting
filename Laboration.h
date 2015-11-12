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
	Color shade(Vec& light, const Vec& cam, Ray& r, HitData& h);
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
	Color shade(Vec& light, const Vec& cam, Ray& r, HitData& h);
};


class LTriangle : public Shape
{
	Vec p1, p2, p3, nor, plane;
	Vec edge0, edge1;
	float d;
public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point) { return plane; }
	LTriangle(Vec _p1, Vec _p2, Vec _p3, Color _color);
	Color shade(Vec& light, const Vec& cam, Ray& r, HitData& h);

};


class LOBB : public Shape
{
public:
	Vec Bcenter;
	Vec Bu;
	Vec Bv;
	Vec Bw;

	Vec Pu, Puo;
	Vec Pv, Pvo;
	Vec Pw, Pwo;



	float halfBu;
	float halfBv;
	float halfBw;

	void test(Ray& ray, HitData& hit);
	Vec normal(Vec& point);

	// Center point, lenght U vector, length V vector, length W vector, color

	LOBB(Vec b, Vec b1, Vec b2, Vec b3, float Hu, float Hv, float Hw, Color _color);
	LOBB(Vec b, float Hu, float Hv, float Hw, Color _color);
};


float angle(Vec& v1, Vec& v2);


Vec cross(const Vec& v1, const Vec& v2);

#endif
