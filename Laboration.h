#ifndef LABORATION_H
#define LABORATION_H
#include "Shapes.h"

class LPlane: public Shape
{
	Vec		n;
	float	d;
public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point);
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


#endif
