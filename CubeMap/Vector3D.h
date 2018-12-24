#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
struct Vector2D
{
	double x, y;

	Vector2D(){x = 0; y = 0;}
	Vector2D(double x, double y){this->x = x; this->y = y;}
};

class Vector3D
{
public:
	double x, y, z;

	Vector3D(){x = 0; y = 0; z = 0;}
	Vector3D(double x, double y, double z){this->x = x; this->y = y; this->z = z;}

	Vector3D operator+ (Vector3D v)
	{
		return Vector3D(x+v.x, y+v.y, z+v.z);
	}

	Vector3D operator- (Vector3D v)
	{
		return Vector3D(x-v.x, y-v.y, z-v.z);
	}

	Vector3D operator* (double v)
	{
		return Vector3D(x*v, y*v, z*v);
	}

	Vector3D operator/(double v)
	{
		return Vector3D(x/v, y/v, z/v);
	}

	Vector3D operator^(Vector3D v)
	{
		Vector3D res;
		double xx = y*v.z - z*v.y;	
		double yy = z*v.x - x*v.z;	
		double zz = x*v.y - y*v.x;	
		return Vector3D(xx, yy, zz);
	}

	Vector3D& operator+=(Vector3D v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vector3D& operator*=(double v)
	{
		x *= v; y *= v; z *= v;
		return *this;
	}

	Vector3D& operator=(Vector3D v)
	{
		x = v.x; y = v.y; z = v.z;
		return *this;
	}

	double& operator[](unsigned int i)
	{
		switch(i)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return x;
		}
	}

	double length()
	{
		return sqrt(x*x+y*y+z*z);
	}

	void normalize()
	{
		double len = length();
		x /= len;
		y /= len;
		z /= len;
	}
};

#endif