/*Classes for vectors in 2 and 3 dimensions*/

#include <iostream>
#include "point.h"

using std::cout;
using std::endl;

class vec2d : public pnt2d
{
public:
    vec2d() : pnt2d() {}
    vec2d( double a, double b ) : pnt2d(a,b) {}
    vec2d(const pnt2d& p) { x=p.x; y=p.y; }

	double length() { return distToOrigin(); }
	double distToPoint( const pnt2d& p);
    double dot(const vec2d& v);
};


class vec3d : public pnt3d
{
public:
    vec3d() : pnt3d() {}
    vec3d( double a, double b, double c ) : pnt3d(a,b,c) { }
    vec3d(const pnt3d& p) { x=p.x; y=p.y; z=p.z; }

	double length() { return distToOrigin(); }
	double distToPoint( const pnt3d& p);
    double dot(const vec3d& v);
    vec3d cross(const vec3d& v);
};
