#include "vector.h"

//Methods for vec2d classes

double vec2d::dot(const vec2d& v)
{
	return co[0]*v.co[0] + co[1]*v.co[1];
}

double vec2d::distToPoint(const pnt2d& p)
{
	return vec2d(p).dot(*this);
}


//Methods for vec3d classes
double vec3d::dot(const vec3d& v)
{
	return co[0]*v.co[0] + co[1]*v.co[1] + co[2]*v.co[2];
}

vec3d vec3d::cross(const vec3d& v)
{
    vec3d c;
    c.co[2] = (co[0] * v.co[1]) - (co[1] * v.co[0]);
    c.co[1] = (co[2] * v.co[0]) - (co[0] * v.co[2]);
    c.co[0] = (co[1] * v.co[2]) - (co[2] * v.co[1]);
    return c;
}
