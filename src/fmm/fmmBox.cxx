#include "fmm.h"

void fmmBox2d::initChildPointers()
{
	for (int i=0;i<4;i++) { children[i] = NULL; }
};

void fmmBox2d::split()
{
	// Add the child boxes
	for (int i=0; i<4;i++) { addChild(i); }
	
	// Transfer objects in this box to the children
	int child;
	for (int i=nPoints-1; i>-1; i--)
	{
		child = getChildIndex(points[i]->co);
		children[child]->points.push_back( points[i] );
		children[child]->nPoints++;
		points.pop_back();
	}
};

int fmmBox2d::getChildIndex(const double* co)
{
	bool posIndex[2];
	for (int i=0; i<2; i++)
	{
		posIndex[i] = (int)(((co[i] - center.co[i])*2.0/length) + 1.0);
	}
	return 1*posIndex[0] + 2*posIndex[1];
};

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
};

fmmBox2d* fmmBox2d::getPointBox(const double* co)
{
	int child = getChildIndex(co);
	if (children[child] != NULL)
		return children[child]->getPointBox(co);
	return this;
};
