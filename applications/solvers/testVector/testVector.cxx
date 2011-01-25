/*! Test function for the dot and cross product of a Vector*/

#include <iostream>
#include <stdlib.h>

#include "primitives.h"

using std::cout;
using std::endl;

int main()
{
	double r;
	// initialize random seed:
	srand ( time(NULL) );
	
	cout << "Testing dot and cross product on Vector class." << endl;
	r = (double)rand()/RAND_MAX;
	cout << "Here is a random double between 0 and 1 : " << r << endl;
	
	cout << " " << endl;
	pnt2d p( (double)rand()/RAND_MAX, (double)rand()/RAND_MAX );
	cout << "Here is a random point : (x,y)=(" << p.co[0] << "," << p.co[1] << ")" << endl;
	cout << "Dist. to origin = " << p.distToOrigin() << endl;
	
	cout << " " << endl;
	pnt2d p2( (double)rand()/RAND_MAX, (double)rand()/RAND_MAX );
	line2d l( p, p2 );
	cout << "Making a line to : (x,y)=(" << l.P1.co[0] << "," << l.P1.co[1] << ")" << endl;
	cout << "Length of line = " << l.length() << endl;
	
	cout << " " << endl;
	vec3d v1( (double)rand()/RAND_MAX, (double)rand()/RAND_MAX, (double)0.0 );
    vec3d v2( (double)rand()/RAND_MAX, (double)rand()/RAND_MAX, (double)0.0 );
	cout << "Here is a random vector v1 : (x,y)=(" << v1.co[0] << "," << v1.co[1] << "," << v1.co[2] << ")" << endl;
    cout << "Here is a random vector v2 : (x,y)=(" << v2.co[0] << "," << v2.co[1] << "," << v2.co[2] << ")" << endl;
	cout << "v1(dot)v2 = " << v1.dot(v2) << endl;
    vec3d c = v1.cross(v2);
    cout << "v1(cross)v2 = (" << c.co[0] << "," << c.co[1] << "," << c.co[2] << ")" << endl;

	return 0;
}
