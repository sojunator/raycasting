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
	Vec lightDirection;
	float ln, colorArr[3];
	Color diffuseLight, ambientLight, shadedColor;

	diffuseLight = { 255, 255, 255 };
	ambientLight = { 50, 50, 50 };

	lightDirection = light - (r.o + r.d*h.t);
	lightDirection.Normalize();

	ln = h.lastNormal.Dot(lightDirection);
	 
	colorArr[0] = ((h.color.r / 255.0f) * (diffuseLight.r / 255.0f) * ln + (ambientLight.r / 255.0f) * (h.color.r / 255.0f)) * 255.0f;
	colorArr[1] = ((h.color.g / 255.0f) * (diffuseLight.g / 255.0f) * ln + (ambientLight.g / 255.0f) * (h.color.g / 255.0f)) * 255.0f;
	colorArr[2] = ((h.color.b / 255.0f) * (diffuseLight.b / 255.0f) * ln + (ambientLight.r / 255.0f) * (h.color.b / 255.0f)) * 255.0f;

	for (int i = 0; i < 3; i++)
	{
		if (colorArr[i] > 255.0f)
			colorArr[i] = 255.0f;
	}

	shadedColor = Color(colorArr[0], colorArr[1], colorArr[2]);

	return shadedColor;
}


LPlane::LPlane(Vec normal, float _d, Color color)
{
	normal.Normalize();
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
		hit.lastNormal = this->n;
		hit.color = this->c;
	}
	else if (t < hit.t && t > 0)
	{
		hit.t = t;
		hit.lastShape = this;
		hit.lastNormal = this->n;
		hit.color = this->c;
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
		hit.lastNormal = this->normal(ray.o + ray.d*hit.t);
		hit.color = this->c;
	}
	else if (t < hit.t && t > 0)
	{
		hit.t = t;
		hit.lastShape = this;
		hit.lastNormal = this->normal(ray.o + ray.d*hit.t);
		hit.color = this->c;
	}

}

// Triangle

LTriangle::LTriangle(Vec _p1, Vec _p2, Vec _p3, Color _color)
{
	p1 = _p1;
	p2 = _p2;
	p3 = _p3;

	edge0 = p2 - p1;
	edge1 = p3 - p1;

	c = _color;
}

Vec LTriangle::normal(Vec &point)
{
	Vec plane = cross(edge0, edge1);
	plane.Normalize();

	return plane;
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
		hit.lastNormal = this->normal(Vec(0, 0, 0));
	}
	else if (t < hit.t && t > 0)
	{
		hit.t = t;
		hit.lastShape = this;
		hit.color = c;
		hit.lastNormal = this->normal(Vec(0,0,0));
	}

}


// motherfucking oob

LOBB::LOBB(Vec b, Vec b1, Vec b2, Vec b3, float Hu, float Hv, float Hw, Color _color)
{
	Bcenter = b;
	Bu = b1;
	Bv = b2;
	Bw = b3;

	Buo = Vec(0, 0, 0) - b1;
	Bvo = Vec(0, 0, 0) - b2;
	Bwo = Vec(0, 0, 0) - b3;

	halfBu = Hu;
	halfBv = Hv;
	halfBw = Hw;


	Pu = Bcenter + (Bu * halfBu);
	Pv = Bcenter + (Bv * halfBv);
	Pw = Bcenter + (Bw * halfBw);


	Pvo = Bcenter - (Bu * halfBu);
	Puo = Bcenter - (Bv * halfBv);
	Pwo = Bcenter - (Bw * halfBw);

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

	Pu = Bcenter + (Bu * halfBu);
	Pv = Bcenter + (Bv * halfBv);
	Pw = Bcenter + (Bw * halfBw);

	Pvo = Bcenter - (Bu * halfBu);
	Puo = Bcenter - (Bv * halfBv);
	Pwo = Bcenter - (Bw * halfBw);

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
			hit.lastNormal = this->normal(ray.o + ray.d*t_min);
		}
		else if (t_min < hit.t && t_min > 0)
		{
			hit.t = t_min;
			hit.lastShape = this;
			hit.color = c;
			hit.lastNormal = this->normal(ray.o + ray.d*t_min);
		}
	}
	else
	{
		if (hit.t == -1 && t_max > 0)
		{
			hit.t = t_max;
			hit.lastShape = this;
			hit.color = c;
			hit.lastNormal = this->normal(ray.o + ray.d*t_max);
		}
		else if (t_max < hit.t && t_max > 0)
		{
			hit.t = t_max;
			hit.lastShape = this;
			hit.color = c;
			hit.lastNormal = this->normal(ray.o + ray.d*t_max);
		}
	}

}

Vec LOBB::normal(Vec& point)
{
	Vec planePoints[6] = { Pu, Pv, Pw, Puo, Pvo, Pwo };
	Vec normalVectors[6] = { Bu, Bv, Bw, Buo, Bvo, Bwo};


	for (int i = 0; i < 6; i++)
	{
		Vec temp = point - planePoints[i];
		temp.Normalize();
		for (int j = 0; j < 6; j++)
		{
			if (temp.Dot(normalVectors[j]) == 0)
			{
				return normalVectors[i];
			}
		}
	}

	return Bw;
}