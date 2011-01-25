#include "fmm.h"

fmmTree2d::fmmTree2d(const mesh2d& mesh)
{
	cout << "Constructing FMM from Mesh" << endl;
	// Reserve space in memory for nPoints
	topBox.nPoints = mesh.nPoints;
	topBox.points.reserve(mesh.nPoints);
	// Add pointers to the points to the topbox array of points
	for (int i=0; i<mesh.nPoints; i++) { topBox.points.push_back( &mesh.points[i] ); }
	// Resize the box to fit around all elements
	resizeTopBox();
};

void fmmTree2d::resizeTopBox()
{
	pnt2d minPoint;
	pnt2d maxPoint;
	
	for (int i=0; i<topBox.nPoints; i++)
	{
		for (int j=0; j<2; j++)
		{
			if (topBox.points[i]->co[j] < minPoint.co[j]) { minPoint.co[j] = topBox.points[i]->co[j]; }
			if (topBox.points[i]->co[j] > maxPoint.co[j]) { maxPoint.co[j] = topBox.points[i]->co[j]; }
		}
		topBox.center.co[0] = (minPoint.co[0] + maxPoint.co[0])/2.0;
		topBox.center.co[1] = (minPoint.co[1] + maxPoint.co[1])/2.0;
		
		double dimX = maxPoint.co[0] - minPoint.co[0]/2.0;
		double dimY = maxPoint.co[1] - minPoint.co[1]/2.0;
		if (dimX > dimY) { topBox.length = dimX; } else {topBox.length = dimY;}
	}
};
