#include "fmm.h"

fmmTree2d::fmmTree2d( mesh2d& mesh, int nCoeffs, int maxEPB, int maxLevel )
{
	cout << "Constructing FMM from Mesh" << endl;
	int i,k;
	
	// Set the number of coefficients
    maxl = maxLevel;
    epb = maxEPB;
	p = nCoeffs;
	topBox.tree = this;
	
	// Initialise the cached binary coefficient matrix
	binomialCoefficients = new double* [2*p];
	for(register int i=0; i<2*p; i++) 
		{ binomialCoefficients[i] = new double [2*p]; }
	for (i=0;i<2*p;i++){ for(k=0;k<2*p;k++){ binomialCoefficients[i][k]=0; }}
	// Cache binomial coefficients
	cacheCoeffs();
	
	// Some points to keep a track of the bounding box
	pnt2d minPoint( mesh.elements[0].points[0]->co[0], mesh.elements[0].points[0]->co[1] );
	pnt2d maxPoint( mesh.elements[0].points[0]->co[0], mesh.elements[0].points[0]->co[1] );
	
	// Add pointers to element sources in the topbox
	int nElements = mesh.elements.size();
	//topBox.targets.reserve( nElements );
	
	for (i=0; i<nElements; i++)
	{
		topBox.elements.push_back( &mesh.elements[i] );
		topBox.elements.back()->minMaxPoints(minPoint, maxPoint);
	}
	
	// Make sure the top-level box fits around the mesh
	resizeTopBox(minPoint, maxPoint);

	// Do the magic - make the tree
	topBox.split( leafBoxes );
};

//Destructor
fmmTree2d::~fmmTree2d()
{
for(int i=0; i<2*p; i++)
{
	delete [] binomialCoefficients[i];
	delete [] binomialCoefficients;
}
};

void fmmTree2d::resizeTopBox(pnt2d& minPoint, pnt2d& maxPoint)
{
	topBox.center.co[0] = (minPoint.co[0] + maxPoint.co[0])/2.0;
	topBox.center.co[1] = (minPoint.co[1] + maxPoint.co[1])/2.0;
	
	double dimX = maxPoint.co[0] - minPoint.co[0]/2.0;
	double dimY = maxPoint.co[1] - minPoint.co[1]/2.0;
	if (dimX > dimY) { topBox.length = dimX; } else {topBox.length = dimY;}
};

void fmmTree2d::writeVTK()
{
	// Create a new mesh object to store the tree geometry and then
	// export that mesh as VTK
	mesh2d treeMesh;
	treeMesh.meshName = "fmmTree";
	
	// Recurse through the tree and add each 
	// box vertices and edges to the treeMesh
	topBox.writeToMesh(treeMesh, true);
	
	// Write the created mesh to VTK format
	treeMesh.writeVTK();
};

void fmmTree2d::multipoleExpansion()
{
	int nLeafs = leafBoxes.size();
	
	cout << "#########" << endl;
	cout << "Number of leaf-boxes = " << nLeafs << endl;
	cout << "#########" << endl;
	
	for (int i=0; i<nLeafs; i++)
	{
		leafBoxes[i]->expandMultipole();
	}
};

void fmmTree2d::upwardPass()
{
	cout << "DOING THE UPWARD PASS" << endl;
	topBox.getChildrenMultipoles();
};

void fmmTree2d::downwardPass()
{
	cout << "DOING THE DOWNWARD PASS" << endl;
	cout << "1) DOING THE COUSIN INTERACTIONS..." << endl;
	topBox.doCousinInteractions();
	cout << "2) DOING THE LOCAL TRANSLATIONS..." << endl;
	topBox.doLocalTranslations();
};

void fmmTree2d::cacheCoeffs()
{
	int i,k;

	// Make Pascals triangle in the binomialCoefficients array
	binomialCoefficients[0][0] = 1.0;
	for (k=1; k<2*p; k++)
	{
		binomialCoefficients[k][0] = 1.0;
		for (i=1;i<k;i++) 
		{ 
			binomialCoefficients[k][i] = 
			 binomialCoefficients[k-1][i] + binomialCoefficients[k-1][i-1]; 
		}
		binomialCoefficients[k][i] = 1.0;
	}

	/* Output the Triangle matrix
	cout << "Caching of coefficients complete! Here is the array!" << endl;
	for (k=0;k<2*p;k++)
	{
		for (i=0;i<2*p;i++)
		{
			cout << binomialCoefficients[k][i] << ", ";
		}
		cout << endl;
	}
	*/
};

int fmmTree2d::binaryCoeff(int n, int k)
{
	//cout << "n=" << n << ", k=" << k << endl;
	// n = row, k = column to look up
	return binomialCoefficients[n][k];
};
