#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;

#include "mesh.h"
#include "fmm.h"

#include "sys/time.h"

double timeDiff(timeval &start, timeval &end)
{
   double ms= (end.tv_usec - start.tv_usec);
   double s = (end.tv_sec - start.tv_sec);
   return (s+(ms/1000000));
};

int main()
{
	timeval start,stop;
	
	// Read in points and faces from a polyMesh directory
	gettimeofday(&start, NULL);
	mesh2d mesh;		// Read the mesh from the 0 directory
	mesh.read();
	gettimeofday(&stop, NULL);
	cout << "Read the mesh in time: " << timeDiff(start, stop) << endl;
	
	// Write the mesh out
	gettimeofday(&start, NULL);
	mesh.writeVTK();	// Write the mesh to VTK format
	gettimeofday(&stop, NULL);
	cout << "Wrote the mesh in: " << timeDiff(start, stop) << endl;
	
	// Construct the FMM-tree for potential elements
	gettimeofday(&start, NULL);
	fmmTree2d tree(mesh, 6);	// Make an fmmTree from the mesh (p=6)
	gettimeofday(&stop, NULL);
	cout << "Constructed the tree in: " << timeDiff(start, stop) << endl;

	// Perform the multipole expansions on leaf boxes
	cout << "Performing multipole expansions:" << endl;
	gettimeofday(&start, NULL);
	tree.multipoleExpansion();	// write the tree to VTK (takes ages for large number of levels)
	gettimeofday(&stop, NULL);
	cout << "Completed multipole expansions in:" << timeDiff(start, stop) << endl;

	// Write the quad-tree to VTK format
	cout << "Writing FMM Tree object:" << endl;
	gettimeofday(&start, NULL);
	tree.writeVTK();	// write the tree to VTK (takes ages for large number of levels)
	gettimeofday(&stop, NULL);
	cout << "Wrote the FMM tree in:" << timeDiff(start, stop) << endl;

	return 0;
};

