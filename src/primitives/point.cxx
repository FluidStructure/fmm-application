#include "point.h"

// 2D point methods
double pnt2d::distToOrigin()
{ 
	return sqrt(pow(x,2.0) + pow(y,2.0));
};

double pnt2d::distToPoint(const pnt2d& p)
{
	return sqrt(pow(x-p.x,2.0) + pow(y-p.y,2.0));
};

//3D point methods
double pnt3d::distToOrigin()
{
	return sqrt(pow(x,2.0) + pow(y,2.0) + pow(z,2.0));
};

double pnt3d::distToPoint(const pnt3d& p)
{
	return sqrt(pow(x-p.x,2.0) + pow(y-p.y,2.0) + pow(z-p.z,2.0));
};
