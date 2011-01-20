#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;

#include "primitives.h"
#include "sources.h"
#include "mesh.h"

int main()
{
	cout << "Testing FMM in 2 and 3 dimensions..." << endl;
	cout << " " << endl;
	
	// Test pointers to pnt2d types
	pnt2d * l;
	cout << l << endl;
	l = new pnt2d(3.6, 8.9);
	cout << l->x << "," << l->y << endl;
	cout << " " << endl;
	
	// Create a 2D point source (biot-savart kernel)
	pnt2d p(5.6, 7.8);
	pointSourceBiot2d s( 3.7 , 4.8 );
	cout << "p.distToOrigin() = " << p.distToOrigin() << endl;
	cout << "s.distToOrigin() = " << s.location->distToOrigin() << endl;
	
	// Read in points and faces from a polyMesh directory
	mesh2d mesh;
	mesh.read();
	
	// Add some random point and line sources
	
	// Output the position of the sources
	
	// Construct the FMM-tree for potential elements
	
	
	return 0;
}

