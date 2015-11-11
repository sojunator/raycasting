#include "Shapes.h"
#include "Laboration.h"

Vec cross(const Vec& v1, const Vec& v2)
{
	Vec returnValue;

	returnValue = Vec(v1.y*v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	return returnValue;

}

float angle(Vec& v1, Vec& v2)
{
	float returnValue = abs(acos(v1.Dot(v2) / (v1.Length() * v2.Length())));
	return returnValue;
}

Color Shape::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	return c;
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

// Sphere

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

// Triangle

LTriangle::LTriangle(Vec _p1, Vec _p2, Vec _p3, Color _color)
{
	// A = p1, B = p2, C = p3
	plane= cross(_p1 - _p2, _p3 - _p2);

	p1 = _p1;
	p2 = _p2;
	p3 = _p3;
	c = _color;

	d = 0 - (plane.x * p1.x + plane.y * p1.y + plane.z * p1.z);
}


void LTriangle::test(Ray& ray, HitData& hit)
{


	float t = 0;
	float over = -d - (plane.Dot(ray.o));
	float under = plane.Dot(ray.d);

	if (under != 0)
	{
		t = over / under;
		// A = p1, B = p2, C = p3 AA = v
		Vec Bat, BA, BC, AX, AAT, AC, V, X, AB, CA, CB, CX, BX;

		X = ray.d*t + ray.o;

		AB = p2 - p1;
		BA = p1 - p2;
		BC = p3 - p2;
		AC = p3 - p1;
		CA = p1 - p3;
		CB = p2 - p3;

		Bat = BC * (BA.Dot(BC) / BC.Dot(BC));
		V = BA - Bat;

		AX = X - p1;
		BX = X - p2;
		CX = X - p3;


		if ((angle(AB, AX) < angle(AB, AC) && (angle(AC, AX) < angle(AB, AC)) 
			&& ((angle(BA, BX) < angle(BA, BC) && (angle(BC, BX) < angle(BA, BC))))
			&& ((angle(CX, CA) < angle(CA, CB) && (angle(CB, CX) < angle(CA, CB))))))
		{

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


	}

}