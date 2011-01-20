/*Classes for lines in 2 and 3 dimensions*/

#include <iostream>
#include "point.h"

using std::cout;
using std::endl;

class line2d
{
public:
	pnt2d P0, P1;
    line2d( pnt2d a, pnt2d b) { P0=a; P1=b; }

    double length() { return P1.distToPoint(P0); };
};


class line3d
{
public:
	pnt3d P0, P1;
	line3d( pnt3d a, pnt3d b) { P0=a; P1=b; }

    double length() { return P1.distToPoint(P0); };
};
