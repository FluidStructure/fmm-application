#include "fmm.h"

fmmTree2d::fmmTree2d(const mesh2d& mesh, int nCoeffs)
{
	cout << "Constructing FMM from Mesh" << endl;
	
	// Set the number of coefficients
	p = nCoeffs;
	topBox.p = nCoeffs;
	topBox.ak = new complex<double>[p];
	
	// Some points to keep a track of the bounding box
	pnt2d minPoint( mesh.elements[0].points[0]->co[0], mesh.elements[0].points[0]->co[1] );
	pnt2d maxPoint( mesh.elements[0].points[0]->co[0], mesh.elements[0].points[0]->co[1] );
	
	// Add pointers to element sources in the topbox
	int nElements = mesh.elements.size();
	topBox.targets.reserve( nElements );
	
	for (int i=0; i<nElements; i++)
	{
		topBox.elements.push_back( &mesh.elements[i] );
		topBox.elements.back()->minMaxPoints(minPoint, maxPoint);
	}
	
	// Make sure the top-level box fits around the mesh
	resizeTopBox(minPoint, maxPoint);

	// Do the magic - make the tree
	topBox.split( leafBoxes );
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
		leafBoxes[i]->expandMultipole( p );
	}
};

void fmmTree2d::upwardPass()
{
	cout << "Upward pass not implemented yet" << endl;
};

void fmmTree2d::downwardPass()
{
	cout << "Downward pass not implemented yet" << endl;
};

void fmmTree2d::nearInteractions()
{
	cout << "Near interactions not implemented yet" << endl;
};
