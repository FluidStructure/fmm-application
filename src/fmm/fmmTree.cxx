#include "fmm.h"

fmmTree2d::fmmTree2d(const mesh2d& mesh)
{
	cout << "Constructing FMM from Mesh" << endl;
	
	// Some points to keep a track of the bounding box
	pnt2d minPoint( mesh.elements[0].points[0]->co[0], mesh.elements[0].points[0]->co[1] );
	pnt2d maxPoint( mesh.elements[0].points[0]->co[0], mesh.elements[0].points[0]->co[1] );
	
	// Add pointers to element sources in the topbox
	int nElements = mesh.elements.size();
	topBox.targets.reserve( nElements );
	
	for (int i=0; i<nElements; i++)
	{
		topBox.elements.push_back( elementInfo( &mesh.elements[i] ) );
		topBox.elements.back().element->minMaxPoints(minPoint, maxPoint, 2);
	}
	cout << "Minimum and maximum points of bounding box are:" << endl;
	cout << "(" << minPoint.co[0] << ", " << minPoint.co[1] << ")" << endl;
	cout << "(" << maxPoint.co[0] << ", " << maxPoint.co[1] << ")" << endl;

	resizeTopBox(minPoint, maxPoint);
	cout << "Top Box in the FMM tree is of size:" << endl;
	cout << "Center : (" << topBox.center.co[0] << "," << topBox.center.co[1] << ")" << endl;
	cout << "Length : " << topBox.length << endl;
	
	// Reserve space in memory for nPoints and Add pointers to targets in the topbox
	//int nPoints = mesh.points.size();
	//topBox.targets.reserve( nPoints );
	//for (int i=0; i<nPoints; i++) { topBox.targets.push_back( &mesh.points[i] ); }
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
