#include "point.h"

// 2D point methods
double pnt2d::distToOrigin()
{ 
	return sqrt(pow(co[0],2.0) + pow(co[1],2.0));
};

double pnt2d::distToPoint(const pnt2d& p)
{
	return sqrt(pow(co[0]-p.co[0],2.0) + pow(co[1]-p.co[1],2.0));
};

//3D point methods
double pnt3d::distToOrigin()
{
	return sqrt(pow(co[0],2.0) + pow(co[1],2.0) + pow(co[2],2.0));
};

double pnt3d::distToPoint(const pnt3d& p)
{
	return sqrt(pow(co[0]-p.co[0],2.0) + pow(co[1]-p.co[1],2.0) + pow(co[2]-p.co[2],2.0));
};
