#include "Shapes.h"
#include "Laboration.h"
#include <utility>
#define EPSILON 0.00000001

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
	Vec plane;
	plane = cross(_p1 - _p2, _p3 - _p2);

	p1 = _p1;
	p2 = _p2;
	p3 = _p3;
	c = _color;

	d = 0 - (plane.x * p1.x + plane.y * p1.y + plane.z * p1.z);
}


void LTriangle::test(Ray& ray, HitData& hit)
{
	Vec e_1, e_2, q, s, r;
	float a, f, u, v, t;

	e_1 = p2 - p1;
	e_2 = p3 - p1;


	q = cross(ray.d, e_2);
	a = e_1.Dot(q);

	if (a > -0.0001 && a < 0.0001)
		return;

	f = 1 / a;
	s = ray.o - p1;
	u = f * (s.Dot(q));

	if (u < 0.0)
		return;

	r = cross(s, e_1);
	v = f * (ray.d.Dot(r));
	
	if (v < 0.0 || (u + v) > 1.0)
		return;

	t = f * (e_2.Dot(r));

	if (hit.t == -1 && t > 0)
	{
		hit.t = t;
		hit.lastShape = this;
		hit.color = c;
	}
	else if (t < hit.t && t > 0)
	{
		hit.t = t;
		hit.lastShape = this;
		hit.color = c;
	}

}

// motherfucking oob

LOBB::LOBB(Vec b, Vec b1, Vec b2, Vec b3, float Hu, float Hv, float Hw, Color _color)
{
	Bcenter = b;
	Bu = b1;
	Bv = b2;
	Bw = b3;

	halfBu = Hu;
	halfBv = Hv;
	halfBw = Hw;

	Pu = Bu * halfBu;
	Pv = Bv * halfBv;
	Pw = Bw * halfBw;

	Pvo = Bv * (-halfBv);
	Puo = Bu * (-halfBu);
	Pwo = Bw * (-halfBw);

	c = _color;
}

LOBB::LOBB(Vec b, float Hu, float Hv, float Hw, Color _color)
{
	Bcenter = b;
	Bu = Vec(1, 0, 0) * Hu;
	Bv = Vec(0, 1, 0) * Hv;
	Bw = Vec(0, 0, 1) * Hw;

	halfBu = Hu;
	halfBv = Hv;
	halfBw = Hw;

	Pu = Bu * halfBu;
	Pv = Bv * halfBv;
	Pw = Bw * halfBw;

	Pvo = Bv * (-halfBv);
	Puo = Bu * (-halfBu);
	Pwo = Bw * (-halfBw);

	c = _color;

}

void LOBB::test(Ray& ray, HitData& hit)
{
	float t_max = INFINITY;
	float t_min = -INFINITY;

	float e, f, t_1, t_2;

	Vec bArr[3];
	float lenghtArr[3];

	bArr[0] = Bu;
	lenghtArr[0] = halfBu;

	bArr[1] = Bv;
	lenghtArr[1] = halfBv;

	bArr[2] = Bw;
	lenghtArr[2] = halfBw;

	Vec p = Bcenter - ray.o;
	
	for (int i = 0; i < 3; i++)
	{
		e = bArr[i].Dot(p);
		f = bArr[i].Dot(ray.d);

		if (abs(f) > 0.0001)
		{
			t_1 = (e + lenghtArr[i]) / f;
			t_2 = (e - lenghtArr[i]) / f;

			if (t_1 > t_2)
				std::swap(t_1, t_2);

			if (t_1 > t_min)
				t_min = t_1;

			if (t_2 < t_max)
				t_max = t_2;

			if (t_min > t_max || t_max < 0)
				return;

		}
		else if (-e - lenghtArr[i] > 0 || -e + lenghtArr[i] < 0)
			return;
	}

	if (t_min > 0)
	{
		if (hit.t == -1 && t_min > 0)
		{
			hit.t = t_min;
			hit.lastShape = this;
			hit.color = c;
		}
		else if (t_min < hit.t && t_min > 0)
		{
			hit.t = t_min;
			hit.lastShape = this;
			hit.color = c;
		}
	}
	else
	{
		if (hit.t == -1 && t_max > 0)
		{
			hit.t = t_max;
			hit.lastShape = this;
			hit.color = c;
		}
		else if (t_max < hit.t && t_max > 0)
		{
			hit.t = t_max;
			hit.lastShape = this;
			hit.color = c;
		}
	}

}

Vec LOBB::normal(Vec& point)
{
	return Vec(0, 0, 0);
}