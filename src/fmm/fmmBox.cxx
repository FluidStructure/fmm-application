#include "fmm.h"

void elementInfo::assignToChildren( const fmmBox2d* box)
{
	int nPoints, index;
	
	// Initializers for edge information 
	// (P1, P2 = intersection points as normalized distance along edge from co[0]->co[1])
	int nChilds = 0;
	int childIndices[4] = {-1};
	
	nPoints = this->element->points.size();
		
	if ( nPoints == 1)
	{
		index = box->getChildIndex( this->element->points[0]->co );
		cout << "Found a point element : childIndex: " << index << "" << endl;
	}
	else if (nPoints == 2)
	{
		cout << "Found a line element : ";
		cout <<  "point1: (" << this->element->points[0]->co[0] << "," << this->element->points[0]->co[1] << ") ";
		cout <<  "point2: (" << this->element->points[1]->co[0] << "," << this->element->points[1]->co[1] << ") " << endl;
		nChilds = box->getChildIndex( 
			this->element->points[0]->co, 
			this->element->points[1]->co,
			childIndices);
		cout << "-->Child Indices";
		for (int i=0; i<nChilds; i++)
		{
			cout << ", " << childIndices[i];
		}
		cout << endl;
	}
	else
		cout << "Found an element with " << nPoints << " points" << endl;
};

//##########################

void fmmBox2d::initPointers()
{
	// Initialize child pointers to NULL
	for (int i=0;i<4;i++) { children[i] = NULL; }
	// Initialize parent pointer to NULL
	parent = NULL;
};

void fmmBox2d::split()
{
	int i, N;
	int childIndices[4];
	
	// Add the children to this parent
	for (i=0; i<4;i++) { addChild(i); }
	
	// Get the children that each element touches
	for (i=elements.size()-1; i>-1; i--)
	{
		elements[i].assignToChildren(this);
		elements.pop_back();		// Delete this element from the parent
	}
	
	/* Transfer targets in this box to the children
	int child;
	for (int i=targets.size()-1; i>-1; i--)
	{
		child = getChildIndex(targets[i]->co);
		children[child]->targets.push_back( targets[i] );
		targets.pop_back();
	}
	*/
	
	/* Remove empty children (point to NULL)
	for (i=0; i<4;i++) 
	{ 
		if (children[i]->elements.size() == 0) 
			{ delete children[i]; children[i]=NULL; }
	}
	*/

};

void fmmBox2d::writeToMesh(mesh2d& mesh, bool children=true)
{
	int i,j,c;
	double x, y;
	int pIndex[4];
	
	mesh.points.reserve( mesh.points.size() + 4 );
	c=0;
	// Write this box
	for (j=-1; j<2; j+=2)
	{
		for (i=-1; i<2; i+=2)
		{
			x = this->center.co[0] + ((double)i * this->length)/2.0;
			y = this->center.co[1] + ((double)j * this->length)/2.0;
			mesh.points.push_back( pnt2d(x, y) );
			pIndex[c]=mesh.points.size()+c; c++;
		}
	}
	mesh.elements.push_back( meshElement( 4, &pIndex[0], mesh.points) );
	
	// Call to write children boxes
	if (children == true)
	{ 
		for (i=0;i<4;i++)
		{
			if (this->children[i] != NULL)
			{
				this->children[i]->writeToMesh(mesh, true);
			}
		}
	}
};

//------------------
// POINTS
int fmmBox2d::getChildIndex(const double* co) const
{
	int index = 0;
	if (co[1] - center.co[1] > 0.0) {index=2;}
	if (co[0] - center.co[0] > 0.0) {index+=1;}
	return index;
};

// EDGES
int fmmBox2d::getChildIndex(const double* co1, const double* co2, int* childIndices) const
{
	// This assumes that the edge fits correctly within the topmost (level 0) box
	double xi, yi, X, Y;
	
	// Get the child boxes that each end-point lie within
	int i1 = this->getChildIndex(co1);
	int i2 = this->getChildIndex(co1);
	
	 // Both points are in the same box
	if (i1 == i2)
		{ childIndices[0] = i1; return 1; }
		
	// Both points are in separate boxed above or below x-axis of box
	else if ((i1 >= 2) == (i2 >= 2))
		{ childIndices[0] = i1; childIndices[1] = i1; return 2; }
		
	// Both points are in separate boxed above or below y-axis of box
	else if ((i1==0 or i1==2) == (i2==0 or i2==2)) 
		{ childIndices[0] = i1; childIndices[1] = i1; return 2; }
		
	// The edge might pass through a few children, get the x and y axis intersections
	X = co2[0] - co1[0]; Y = co2[1] - co1[1];
	yi = ((center.co[0] - co1[0])/X)*Y - (center.co[1]-co1[1]);
	xi = ((center.co[1] - co1[1])/Y)*X - (center.co[0]-co1[0]);
	
	// Get the primary quadrant that this edge is within (0 ,1 ,2, 3)
	// Add this quadrant box to the list of children that edge passes through 
	// (must at least pass through this box)
	int Qx=0; int Qy=0;
	if (xi>0.0) {Qx=1;}
	if (yi>0.0) {Qy=1;}
	int N = 0; childIndices[N] = Qx+(Qy*2);
	
	// If the x/y intercept is within the box then add the quadrant 
	// above or below to the children that the edge passes through
	double L = length/2.0;
	if (fabs(xi) < L) { N++; childIndices[N] = childIndices[0] + 2 - Qy*4; }
	if (fabs(yi) < L) { N++; childIndices[N] = childIndices[0] + 1 - Qx*2; }
	
	return N;
};

//------------------

void fmmBox2d::addChild(const unsigned i)
{
	// Get binary 0 or 1 index of child position from integer index
	int x = (i&(1<<0))>>0;
	int y = (i&(1<<1))>>1;
	
	// Add the child at this index position
	children[i] = new fmmBox2d();
	children[i]->center.co[0] = center.co[0] + ((double)x-0.5)*length/2.0;
	children[i]->center.co[1] = center.co[1] + ((double)y-0.5)*length/2.0;
	children[i]->length = length/2.0;
	children[i]->level = this->level + 1;
	children[i]->parent = this;
};

fmmBox2d* fmmBox2d::getPointBox(const double* co)
{
	int child = getChildIndex(co);
	if (children[child] != NULL)
		return children[child]->getPointBox(co);
	return this;
};
