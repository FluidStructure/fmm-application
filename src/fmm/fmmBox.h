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

class fmmBox2d;	// Forward declaration 
class elementInfo
{
public:
	const meshElement* element;
	vector<double> intersections;
	
	void assignToChildren(const fmmBox2d* box);
	
	// Constructors
	elementInfo() { element=NULL; }
	elementInfo(const meshElement* mElement) { element=mElement; }
	// Destructors
	~elementInfo() {};
};

class fmmBox2d
{
private:
	void initPointers();
	
public:
	fmmBox2d* parent;
	fmmBox2d* children[4];
	pnt2d center;
	double length;
	int level;
	
	// Vector of elements and info stored in this box
	vector<const pnt2d*> targets;
	vector<elementInfo> elements;
	
	// Methods for getting the child boxes of points, edges and faces
	int getChildIndex(const double* co) const;
	int getChildIndex(const double* co1, const double* co2, int* childIndices) const;
	
	// Methods for adding children, creating and outputting the tree
	void addChild(const unsigned i);
	void split();
	void writeToMesh(mesh2d& mesh, bool children);

	// Recursive call to get smallest box in which a point resides
	fmmBox2d* getPointBox(const double* co);
	
	// Constructors
	fmmBox2d() { initPointers(); center.co[0] = 0.0; center.co[1] = 0.0; length=0.0; }
	fmmBox2d( double a, double b, double c) { initPointers(); center.co[0]=a; center.co[1]=b; length=c; }
	// Destructors
	~fmmBox2d() {};
};

#endif
