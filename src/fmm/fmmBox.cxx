#include "fmm.h"

//##########################

// Destructor
fmmBox2d::~fmmBox2d()
{
	delete [] ak;
	delete [] bl;
};

void fmmBox2d::initPointers()
{
	// Initialize child pointers to NULL
	for (int i=0;i<4;i++) { children[i] = NULL; }
	// Initialize parent pointer to NULL
	parent = NULL;
	// Initialize poiters to FMM Storage
	ak = NULL;
	bl = NULL;
};

void fmmBox2d::split( vector<fmmBox2d*>& leafs )
{
	int i, N;
	int childIndices[4];
	
	// Add the children to this parent
	for (i=0; i<4;i++) { addChild(i); }
	
	// Get the children that each element touches
	for (i=elements.size()-1; i>-1; i--)
	{
		assignToChildren(elements[i]);
		elements.pop_back();		// Delete this element from the parent
	}

	// Look through the children and if any of them meet the criteria
	// then split them too (recursive call)
	// OR: if they don't have any elements then point them to NULL
	for (i=0; i<4; i++)
	{
		if ((children[i]->elements.size() > tree->epb) and ( children[i]->level < tree->maxl ))
		{
			children[i]->split( leafs );
		}
		else if (children[i]->elements.size() == 0) 
		{
			delete children[i]; 
			children[i]=NULL; 
		}
		else
		{
			leafs.push_back( children[i] );		// This child box is a leaf
		}
	}
};

void fmmBox2d::writeToMesh(mesh2d& mesh, bool children=true)
{
	int i,j,c,s;
	double x, y;
	int pIndex[4];
	
	mesh.points.reserve( mesh.points.size() + 4 );
	s = mesh.points.size();
	c=0;
	// Write this box
	for (j=-1; j<2; j+=2)
	{
		for (i=-1; i<2; i+=2)
		{
			x = this->center.co[0] + ((double)i * this->length)/2.0;
			y = this->center.co[1] + ((double)j * this->length)/2.0;
			mesh.points.push_back( elementPoint(x, y) );
			pIndex[c]=s+c; c++;
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

void fmmBox2d::assignToChildren(meshElement* element)
{
	int nPoints = element->points.size();
	int nChilds = 0;
	int childIndices[4] = {-1};
		
	if ( nPoints == 1)
	{
		childIndices[0] = this->getChildIndex( element->points[0]->co );
		nChilds = 1;
	}
	else if (nPoints == 2)
	{
		nChilds = this->getChildIndex( 
			element->points[0]->co, element->points[1]->co, childIndices);
	}
	else
	{
		cout << "WARNING: Found an element with " << nPoints << " points while splitting box" << endl;
	}
	
	// Copy this element to the children
	for (int i=0; i<nChilds; i++)
	{
		this->children[childIndices[i]]->elements.push_back( element );
	}
};

//------------------
// Methods to get a pointer to a box that a point is inside of

/* TODO:  Check that point is inside of FMM bounding box*/ 
fmmBox2d* fmmBox2d::getPointBox(const double* co)
{
	int child = getChildIndex(co);
	if (children[child] != NULL)
		return children[child]->getPointBox(co);
	return this;
};

fmmBox2d* fmmBox2d::getPointBox(const double* co, int atLevel)
{
	int child = getChildIndex(co);
	if ((children[child] != NULL) and (level < atLevel))
		{ return children[child]->getPointBox(co, atLevel); }
	// Return NULL if the atLevel was not reached
	if (level == atLevel) { return this; }
	else { return NULL; }
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
	int i2 = this->getChildIndex(co2);
	
	// Both points are in the same box
	if (i1 == i2)
		{ childIndices[0] = i1; return 1; }
		
	// Both points are in separate boxed above or below x-axis of box
	else if ((i1 >= 2) == (i2 >= 2))
		{ childIndices[0] = i1; childIndices[1] = i2; return 2; }
	// Both points are in separate boxed above or below y-axis of box
	else if ((i1==0 or i1==2) == (i2==0 or i2==2)) 
		{ childIndices[0] = i1; childIndices[1] = i2; return 2; }
	
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
	N++;
	
	return N;
};

//----------------------

void fmmBox2d::expandMultipole()
{
	int nPoints = 0;
	int nElements = elements.size();
	double lims[2] = {0.0, 1.0};
	ak = new complex<double>[tree->p];
	
	for (int i=0; i<nElements; i++)
	{
		nPoints = elements[i]->points.size();
		if (nPoints == 1)
		{
			elements[i]->expandMultipole( this->center.co, ak, tree->p );
		}
		else if (nPoints == 2)
		{
			// Get limits of line-integral (line-box intersection points)
			lineIntersectionPoints( elements[i], lims );
			elements[i]->expandMultipole( this->center.co, lims, ak, tree->p );
		}
		else
		{
			cout << "Higher order elements not supported in 2d" << endl;
		}
	}
};

// Recursive call to get translate multipoles from children to parents
void fmmBox2d::getChildrenMultipoles()
{
	for (int i=0; i<4; i++)
	{
		if (children[i] != NULL)
		{
			if (children[i]->ak == NULL) 
			{ 
				children[i]->getChildrenMultipoles(); 
			}
			children[i]->translateMultipole();
		}	
	}
};

// Recursive call to calculate cousin (far-field) interactions
void fmmBox2d::doCousinInteractions()
{
	//cout << "doCousinInteractions for box: level="
	//	<< level << ", (x,y)=(" 
	//	<< center.co[0] << "," << center.co[1] << ")" << endl;
	
	// Initialise the local expansion coefficient array for this box
	int p = tree->p;
	if (bl == NULL)
	{
		bl = new complex<double>[p];
		for (int i=0;i<p;i++) {bl[i]=complex<double>(0,0);}
	}
	
	// Get the cousin-interactions for this particular box
	fmmBox2d* cousins[27] = {NULL};
	getCousins(cousins);
	for (int i=0;i<27;i++)
	{
		if (cousins[i] != NULL)
		{
			//cout << "-- found cousin at: level=" << cousins[i]->level << ", (x,y)=("
			//<< cousins[i]->center.co[0] << "," << cousins[i]->center.co[1] << ")" << endl;
			
			toLocalExpansion(cousins[i]);
		}
	}
	
	// Now ask the children to do the same
	int numChildren = 0;
	for (int i=0;i<4;i++)
	{
		if (children[i] != NULL) 
		{ 
			children[i]->doCousinInteractions();
			numChildren+=1;
		}
	}
};

// Recursive call to translate local expansions to children
void fmmBox2d::doLocalTranslations()
{
	int numChildren = localExpansionToChildren();
	
	// But if this is a leaf-box then it should do the interactions
	// with its neighbours right now OTHERWISE continue recursive call
	// to children
	if (numChildren > 0) 
	{	
		for (int i=0;i<4;i++)
		{
			if (children[i] != NULL)
			{
				children[i]->doLocalTranslations();
			}
		}
	}
	else 
	{ 
		//cout << "Doing a direct interaction calc" << endl;
		doDirectInteractionsBothWays();
		//cout << "Completed direct interaction calc" << endl;
		
		//cout << "Doing a local expansion to target calc" << endl;
		localExpansionToTargetPotential();
		localExpansionToTargetVelocity();
		//cout << "Completed local expansion to target calc" << endl;
	}
};

void fmmBox2d::localExpansionToTargetPotential()
{
	for (int t=0;t<elements.size();t++)
	{
		meshElement* target = elements[t];
		
		if ( pointInBox(target->collocationPoint().co) )
		{
			complex<double> z( 
				target->collocationPoint().co[0] - center.co[0],
				target->collocationPoint().co[1] - center.co[1]);
			
			complex<double> potential(0.0,0.0);
			for (int l=0;l<tree->p;l++)
				{ potential += bl[l]*pow(z,l); }
			
			target->potential += potential.real();
		}
	}
};

void fmmBox2d::localExpansionToTargetVelocity()
{
	for (int t=0;t<elements.size();t++)
	{
		meshElement* target = elements[t];

		if ( pointInBox(target->collocationPoint().co) )
		{
			complex<double> z( 
				target->collocationPoint().co[0] - center.co[0],
				target->collocationPoint().co[1] - center.co[1]);
			
			// TODO: WATCH THIS SPACE - POSSIBLE ERROR
			complex<double> velocity(0.0,0.0);
			for (int l=0;l<tree->p;l++)
				{ velocity += (double)l*bl[l]*pow(z,l-1); }
			
			target->velocity[0] += -1.0*velocity.imag();
			target->velocity[1] += velocity.real();
		}
	}
};

void fmmBox2d::doDirectInteractionsBothWays()
{
	meshElement* target;
	int e,t;
	int ne = elements.size();
	bool returnFavour = true;
	
	
	fmmBox2d* neighbours[8] = {NULL};
	getNeighbours(neighbours);
	
	// Do the interaction calculations in each of the neighbours that exist
	for (int i=0;i<8;i++)
	{
		if (neighbours[i] != NULL) 
		{ 
			// First go through each element in this box and pass its influence
			// to the targets in the neighbour boxes
			for (e=0; e<ne; e++)
			{
				returnFavour = true;
				if (pointInBox(elements[e]->collocationPoint().co))
					{ returnFavour=false; }
				
				if (not neighbours[i]->hasChild())
					{ returnFavour=false; }
				
				neighbours[i]->elementToTargets( elements[e], returnFavour );
			}
		}
	}
};

bool fmmBox2d::hasChild()
{
	for (int i=0;i<4;i++)
	{
		if (children[i] != NULL) { return true ; }
	}
	return false;
};

void fmmBox2d::elementToTargets( meshElement* element, bool returnFavour)
{
	int ne = elements.size();
	meshElement* target;
	
	// Do direct interactions between element and targets in this box
	for (int t=0;t<ne;t++)
	{
		target = elements[t];
		if ( pointInBox(target->collocationPoint().co) )
		{
			element->directPotential( target );
			element->directVelocity( target );
		}
		
		if (returnFavour==true)
		{
			target->directPotential( element ); 
			target->directVelocity( element ); 
		}
	}
	// Recurse into child boxes doing the same
	for (int i=0;i<4;i++)
	{
		if (children[i] != NULL) 
		{
			children[i]->elementToTargets( element, returnFavour );
		}
	}
};

void fmmBox2d::elementsToTarget( meshElement* target )
{
	// Do direct interactions between elements and a given target
	for (int e=0;e<elements.size();e++)
	{
		elements[e]->directPotential( target ); 
		elements[e]->directVelocity( target ); 
	}
	// Recurse into child boxes doing the same
	for (int i=0;i<4;i++)
	{
		if (children[i] != NULL)
		{
			children[i]->elementsToTarget( target ); 
		}
	}
};

void fmmBox2d::translateMultipole()
{
	complex<double> zo( 
	center.co[0] - parent->center.co[0],
	center.co[1] - parent->center.co[1]);
	
	int p = tree->p;
	if (parent->ak == NULL)
	{
		parent->ak = new complex<double>[p];
		for (int i=0;i<p;i++) {parent->ak[i]=complex<double>(0,0);}
	}
		
	// Lemma 4.6 (Translation of a multipole expansion)
	parent->ak[0] += ak[0];
	for (int l=1; l<p; l++)
	{
		parent->ak[l] += -1.0*ak[0]*pow(zo,l)/(double)l;
		for (int k=1; k<l+1; k++)
		{
			parent->ak[l] += ak[k]*pow(zo,(l-k))*(double)tree->binaryCoeff(l-1,k-1);
		}
	}
};

void fmmBox2d::toLocalExpansion(fmmBox2d* box)
{
	complex<double> zo( 
		box->center.co[0] - center.co[0],
		box->center.co[1] - center.co[1]);
	
	int p = tree->p;
		
	// Lemma 4.7 (Conversion of a multipole expansion into a local expansion)
	//Equation 4.18 - calculating b0        
	bl[0] += box->ak[0]*log(-1.0*zo);
	for (int k=1; k<p; k++)
	{
		bl[0] += box->ak[k]*pow(-1.0,k)/pow(zo,k);
	}
	//Equation 4.19 - calculation bl
	for (int l=1; l<p; l++)
	{
		bl[l] += -1.0*box->ak[0]/((double)l*pow(zo,l));
		complex<double> tmp(0.0,0.0);
		for (int k=1;k<p;k++)
		{
			tmp += ((box->ak[k])/pow(zo,k))*pow(-1.0,k)*(double)tree->binaryCoeff(l+k-1,k-1);
		}
		bl[l] += (1.0/pow(zo,l))*tmp;
	}
};

int fmmBox2d::localExpansionToChildren()
{
	int numChildren=0;
	for (int i=0;i<4;i++)
	{
		if (children[i] != NULL) 
		{ 
			children[i]->translateLocalExpansion(this);
			numChildren+=1;
		}
	}
	return numChildren;
};

// Lemma 4.8 (Translation of a local expansion)
void fmmBox2d::translateLocalExpansion(fmmBox2d* box)
{
	complex<double> zo( 
		box->center.co[0] - center.co[0],
		box->center.co[1] - center.co[1]);
		
	int l,k;
	int p = tree->p;
	
	// First make a copy of the vector
	complex<double> blt[p];
	for (l=0;l<p;l++) { blt[l] = box->bl[l]; }

	// Do a Horner scheme translating the polynomial (Equation 4.21->4.22)
	for (l=0;l<p;l++)
	{
		for (k=(p-1-l);k<(p-1);k++)
		{
			blt[k] = blt[k] - zo*blt[k+1];
		}
	}
	
	// Append this translated local expansion coefficients to this box's bl coefficients
	for (l=0;l<p;l++) { bl[l] += blt[l]; }
};

// Get pointers to the neighbours of this box
void fmmBox2d::getNeighbours(fmmBox2d* neighbourList[])
{
	pnt2d p;
	int i = 0;
	for (int row=-1;row<2;row++)
	{
		for (int col=-1;col<2;col++)
		{
			if ((row!=0) or (col!=0))
			{
				p.co[0] = (double)col*length + center.co[0];
				p.co[1] = (double)row*length + center.co[1];
				// Get the box that is there at this level (if any)
				if (tree->topBox.pointInBox(p.co))
				{ neighbourList[i] = tree->topBox.getPointBox(p.co, level); }
				else { neighbourList[i] = NULL; }
				i += 1;
			}
		}
	}
};

// Get pointers to the cousins of this box
// (Children of parents' neighbours that are not actual neighbours)
void fmmBox2d::getCousins(fmmBox2d* cousinsList[])
{
	// First get the index of this particular box
	if (parent == NULL) { return; }
	int index = parent->getChildIndex(center.co);
	// Set search limits based upon where we are in this parent box
	int rowLims[2] = {-2,4};
	int colLims[2] = {-2,4};
	if (index > 1) { rowLims[0]=-3;rowLims[1]=3; index-=2; }
	if (index > 0) { colLims[0]=-3;colLims[1]=3; }

	pnt2d p;
	int i = 0;	
	for (int row=rowLims[0];row<rowLims[1];row++)
	{
		for (int col=colLims[0];col<colLims[1];col++)
		{
			// This if-statement is to avoid nearest neighbours
			if ((row<-1 or row>1) or (col<-1 or col>1))
			{
				p.co[0] = (double)col*length + center.co[0];
				p.co[1] = (double)row*length + center.co[1];
				
				//cout << "---searchPoint=(" << p.co[0] << "," << p.co[1]
				//<< "), pointInTopBox=" << tree->topBox.pointInBox(p.co) << endl;
				
				// Get the box that is there at this level
				if (tree->topBox.pointInBox(p.co))
				{ cousinsList[i] = tree->topBox.getPointBox(p.co, level); }
				else { cousinsList[i] = NULL; }
				i += 1;
			}
		}
	}
};

int fmmBox2d::lineIntersectionPoints( const meshElement* element, double pints[] )
{	
	double p, D, P, boxEdge;
	pints[0] = 0.0;	pints[1] = 1.0;
	
	double tolu = length*1e-12;
	double toll = -1.0*tolu;
	
	bool p0inside = true;
	
	int i=0;
	int s=0;
	bool j=0;
	
	// Calculate the box limits
	double halfLength = length/2.0;
	double boxLims[2][2];
	boxLims[0][0] = center.co[0] - halfLength;
	boxLims[0][1] = center.co[0] + halfLength;
	boxLims[1][0] = center.co[1] - halfLength;
	boxLims[1][1] = center.co[1] + halfLength;
	
	int nInts = 0;
	for (i=0;i<2;i++)
	{
		j = (i==0);	// "j" is the other coordinate axis index
		
		D = (element->points[1]->co[i] - element->points[0]->co[i]);
		// Avoid a possible divide-by-zero error here
		if ((D < tolu) && (D > toll)) { continue; }
		
		for (int s=0;s<2;s++)
		{
			boxEdge = boxLims[i][s];
			
			p = (boxEdge - element->points[0]->co[i])/D;
			// Check if intersection point p is on the cutting line somewhere
			if ((p <= 0.0) or (p >= 1.0)) { continue; }
			
			// Check if intersection point p is on the box perimeter
			P = (element->points[1]->co[j] - element->points[0]->co[j])*p + element->points[0]->co[j];
			if ( (P < boxLims[j][0]) or (P > boxLims[j][1]) ) { continue; }

			if (nInts > 1) { cerr << "ERROR: More than two intersections found."
								  << " This will probably seg-fault" << endl; }
			pints[nInts] = p;
			nInts ++;
			
			// Check if point[0] is on the in-side of the boxEdge
			if ((element->points[0]->co[i] - boxEdge)*(2*s-1) > 0.0)
				{ p0inside = false; }
		}
	}
	if (nInts == 0) { return nInts; }
	
	if (nInts == 1)
	{
		if (p0inside) { pints[1] = pints[0]; pints[0] = 0.0; return nInts; }
		return nInts;
	}
	
	if (pints[1] < pints[0]) { p = pints[0]; pints[0] = pints[1]; pints[1] = p; }
	return 2;
};

bool fmmBox2d::pointInBox( double co[] )
{
	for (int i=0;i<2;i++)
	{
		if ( abs(co[i]-center.co[i]) >= (length/2.0) ) { return false; }
	}
	return true;
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
	children[i]->level = level + 1;
	children[i]->parent = this;
	children[i]->tree = tree;
};


