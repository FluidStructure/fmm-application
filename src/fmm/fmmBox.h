#ifndef FMMBOX_H_
#define FMMBOX_H_

#include "fmmTree.h"
#include "primitives.h"
#include "mesh.h"
#include <list>
#include <vector>

#include <iostream>
#include <stdlib.h>
using namespace std;

class fmmBox2d
{
private:
	void initChildPointers();
public:
	pnt2d center;
	double length;
	int level;
	
	// Number and list of elements stored in this box
	int nPoints;
	vector<const pnt2d*> points;
	
	//Children is an array of pointers to more boxes
	fmmBox2d* children[4];
	
	void split();
	void addChild(const unsigned i);
	int getChildIndex(const double* co);
	
	// Recursive call to get smallest box in which a point resides
	fmmBox2d* getPointBox(const double* co);
	
	// Constructors
	fmmBox2d() { initChildPointers(); center.co[0] = 0.0; center.co[1] = 0.0; length=0.0; nPoints=0; }
	fmmBox2d( double a, double b, double c) { initChildPointers(); center.co[0]=a; center.co[1]=b; length=c; nPoints=0; }
	// Destructors
	~fmmBox2d() {};
};

#endif
