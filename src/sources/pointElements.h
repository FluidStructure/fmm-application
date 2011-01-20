// Classes to hold point element information in 2D and 3D//

#include "primitives.h"


// Generic 2d point source (biot-savart kernel)
class pointSourceBiot2d
{
public:
	pnt2d * location;										// Pointer to a 2D-point
	double srength0;										// Strength of the source element
	double potential ( const pnt2d& p );					// Potential at a 2d-point in space
	double fmmCutoffRadius() { return 0.0; }				// Define the cut-off radius
	// Constructors
	pointSourceBiot2d () { location = new pnt2d; }			// If no point is given then make a new one
	pointSourceBiot2d (pnt2d& p) { location = &p; }			// If a point is given then point to it
	pointSourceBiot2d (double a, double b) { location = new pnt2d; location->x = a; location->y = b; }	
	// Destructors
	~pointSourceBiot2d() {}
};
