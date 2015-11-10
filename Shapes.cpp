#include "Shapes.h"
#include "Laboration.h"


Color Shape::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	return c;
}

Vec LPlane::normal(Vec &point)
{
	return n;
}

LPlane::LPlane(Vec normal, float _d, Color color)
{
	n = normal;
	d = _d;
	c = color;
}

void LPlane::test(Ray& ray, HitData& hit)
{
	float t = 0;
	float over = -d - (n.Dot(ray.o));
	float under = n.Dot(ray.d);

	if (under != 0)
		t = over / under;


	if (hit.t == -1 && t > 0)
	{
		hit.t = t;
		hit.lastShape = this;
	}
	else if (t < hit.t && t > 0)
	{
		hit.t = t;
		hit.lastShape = this;
	}
}

LSphere::LSphere(Vec _center, float _radius, Color _color)
{
	center = _center;
	radius = _radius;
	radius2 = _radius;
	c = _color;
}

Vec LSphere::normal(Vec &point)
{
	Vec normal = point - center;
	normal.Normalize();
	return normal;
}

void LSphere::test(Ray& ray, HitData& hit)
{
	float b = ray.d.Dot(ray.o - center);
	float c = (ray.o - center).Dot(ray.o - center) - (radius*radius);
	float t_1 = -b + sqrt((b*b - c));
	float t_2 = -b - sqrt((b*b - c));

	float t = t_2;
	if (t_1 < t_2)
		t = t_1;

	if (hit.t == -1 && t > 0)
	{
		hit.t = t;
		hit.lastShape = this;
	}
	else if (t < hit.t && t > 0)
	{
		hit.t = t;
		hit.lastShape = this;
	}

}