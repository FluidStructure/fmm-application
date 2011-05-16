/* Classes for points in 2 and 3 dimensions*/

#ifndef MESH2D_H_
#define MESH2D_H_

#include "primitives.h"
#include <vector>

#include <complex>

#include <iostream>
#include <fstream>
#include <string>

#include <stdlib.h>
using namespace std;

// Define an element who knows what is attached to it
class meshElement; // Forward declaration of meshElement
class elementPoint : public pnt2d
{
public:
	// Relational data
	vector<meshElement*> owners;
	// Values stored at this point
	double value;
	// Constructors
	elementPoint() { co[0]=0; co[1]=0; value=0.0; }
	elementPoint( double a, double b) { co[0]=a; co[1]=b; value=0.0; }
    // Destructor
	~elementPoint() {};
};

//--------------

class meshElement
{
public:
	vector<elementPoint*> points;
	vector<int> pointsIndices;
	int elementType;
	
	// Storage related to this element being a target 
	// (collocationPoint, potential, velocity, etc.)
	pnt2d collocationPoint();
	double potential;
	double velocity[2];
	void clearValues();
	
	void minMaxPoints ( pnt2d& minPoint, pnt2d& maxPoint) const;
	
	// FMM operations
	void expandMultipole ( double zo[], complex<double> ak[], int& p ) const;
	void expandMultipole ( double zo[], double lims[], complex<double> ak[], int& p ) const;

    void directPotential ( meshElement* );
    void directVelocity ( meshElement* );
	
	// Constructors
	meshElement() { elementType=0; }
	meshElement( int nPoints, int* pIndex, vector<elementPoint>& allPoints);
	// Destructors
	~meshElement() {};
};

//--------------

class mesh2d
{
private:
	// A template function for reading values of arbitrary type
	template <class T>
	int readListLine(string& line, T* values, int n);
	
	void removeComments(string& line);
	int getListLength(ifstream& fin);

	void readPoints();
	void readElements();
	void readValues();
public:
	double time;
	string meshName;
    
    void read();
    void writeVTK();

	vector<elementPoint> points;
	vector<meshElement> elements;
	
	// Constructors
	mesh2d() { time=0.0; meshName="mesh"; }
	// Destructors
	~mesh2d() {};
};

#endif
