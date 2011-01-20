/* Classes for points in 2 and 3 dimensions*/

#ifndef POINT_H_
#define POINT_H_

#include <math.h>

class pnt2d
{
public:
	double x, y;
	double distToOrigin(); 
	double distToPoint(const pnt2d& p);
	// Constructors 
	pnt2d() { x=y=0; }
	pnt2d( double a, double b) { x=a; y=b; }
	// Destructor
	~pnt2d() {};
};

class pnt3d : public pnt2d
{
public:
	double z;
	double distToOrigin();
	double distToPoint(const pnt3d& p);
	// Constructors 
	pnt3d() { z=0; }
	pnt3d( double a, double b, double c) : pnt2d(a,b) { z=c; }
	// Destructor
	~pnt3d() {};
};

#endif
