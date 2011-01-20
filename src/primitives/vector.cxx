#include "vector.h"

//Methods for vec2d classes

double vec2d::dot(const vec2d& v)
{
	return x*v.x + y*v.y;
}

double vec2d::distToPoint(const pnt2d& p)
{
	return vec2d(p).dot(*this);
}


//Methods for vec3d classes
double vec3d::dot(const vec3d& v)
{
	return x*v.x + y*v.y + z*v.z;
}

vec3d vec3d::cross(const vec3d& v)
{
    vec3d c;
    c.z = (x * v.y) - (y * v.x);
    c.y = (z * v.x) - (x * v.z);
    c.x = (y * v.z) - (z * v.y);
    return c;
}
