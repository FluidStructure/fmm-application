#ifndef FMMBOX_H_
#define FMMBOX_H_

#include "mesh.h"
#include "primitives.h"

#include <vector>
#include <complex>

#include <iostream>
#include <stdlib.h>
using namespace std;

// Forward declaration of fmmTree2d
class fmmTree2d;

// Declaration of fmmBox2d
class fmmBox2d
{
private:
	void initPointers();
	
public:
	// Pointers to relational data structures
	fmmTree2d* tree;
	fmmBox2d* parent;
	fmmBox2d* children[4];
	
	// Info about this particular box
	pnt2d center;
	double length;
	int level;
	
	// FMM Storage - coefficents for multipole and local expansions
	complex<double> * ak;
	complex<double> * bl;
	
	// Vector of elements and info stored in this box
	//vector<meshElement*> targets;
	vector<meshElement*> elements;
	
	// Methods for getting the child boxes of points, edges and faces
	bool hasChild();
	void assignToChildren(meshElement* element);
	int getChildIndex(const double* co) const;
	int getChildIndex(const double* co1, const double* co2, int* childIndices) const;
	
	// Methods for adding children, creating and outputting the tree
	void addChild(const unsigned i);
	void split( vector<fmmBox2d*>& leafs );
	void writeToMesh(mesh2d& mesh, bool children);

	// Recursive call to get smallest box in which a point resides
	int lineIntersectionPoints( const meshElement* element, double pints[] );
	bool pointInBox( double co[] );
	fmmBox2d* getPointBox(const double* co);
	fmmBox2d* getPointBox(const double* co, int atLevel);
	
	// Relational methods
	void getCousins(fmmBox2d* cousinsList[]);
	void getNeighbours(fmmBox2d* neighbourList[]);
	
	// FMM Methods
	void expandMultipole();
	void getChildrenMultipoles();
	void translateMultipole();
	void toLocalExpansion(fmmBox2d* box);
	void translateLocalExpansion(fmmBox2d* box);
	int localExpansionToChildren();
	void doCousinInteractions();
	void doLocalTranslations();
	void doDirectInteractionsBothWays();
	void localExpansionToTargetPotential();
	void localExpansionToTargetVelocity();
	
	void elementToTargets( meshElement* element, bool returnFavour );
	void elementsToTarget( meshElement* target );
	
	// Constructors
	fmmBox2d() { initPointers(); center.co[0] = 0.0; center.co[1] = 0.0; length=0.0; level=0; }
	fmmBox2d( double a, double b, double c) { initPointers(); center.co[0]=a; center.co[1]=b; length=c; level=0; }
	// Destructors
	~fmmBox2d();
};

#endif
