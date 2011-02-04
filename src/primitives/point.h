/* Classes for points in 2 and 3 dimensions*/

#ifndef POINT_H_
#define POINT_H_

#include <math.h>
using namespace std;

class pnt2d
{
public:
	double co[2];
	//double x, y;
	double distToOrigin(); 
	double distToPoint( pnt2d* p );
	// Constructors 
	pnt2d() { co[0]=0; co[1]=0; }
	pnt2d( double a, double b) { co[0]=a; co[1]=b; }
	// Destructor
	~pnt2d() {};
};

class pnt3d : public pnt2d
{
public:
	double co[3];
	//double z;
	double distToOrigin();
	double distToPoint( pnt3d* p );
	// Constructors 
	pnt3d() { co[2]=0; }
	pnt3d( double a, double b, double c) : pnt2d(a,b) { co[2]=c; }
	// Destructor
	~pnt3d() {};
};

#endif
