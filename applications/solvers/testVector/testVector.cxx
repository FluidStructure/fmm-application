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
	cout << "Here is a random point : (x,y)=(" << p.x << "," << p.y << ")" << endl;
	cout << "Dist. to origin = " << p.distToOrigin() << endl;
	
	cout << " " << endl;
	pnt2d p2( (double)rand()/RAND_MAX, (double)rand()/RAND_MAX );
	line2d l( p, p2 );
	cout << "Making a line to : (x,y)=(" << l.P1.x << "," << l.P1.y << ")" << endl;
	cout << "Length of line = " << l.length() << endl;
	
	cout << " " << endl;
	vec3d v1( (double)rand()/RAND_MAX, (double)rand()/RAND_MAX, (double)0.0 );
    vec3d v2( (double)rand()/RAND_MAX, (double)rand()/RAND_MAX, (double)0.0 );
	cout << "Here is a random vector v1 : (x,y)=(" << v1.x << "," << v1.y << "," << v1.z << ")" << endl;
    cout << "Here is a random vector v2 : (x,y)=(" << v2.x << "," << v2.y << "," << v2.z << ")" << endl;
	cout << "v1(dot)v2 = " << v1.dot(v2) << endl;
    vec3d c = v1.cross(v2);
    cout << "v1(cross)v2 = (" << c.x << "," << c.y << "," << c.z << ")" << endl;

	return 0;
}
