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
	timeval start,stop,start1,stop1;
	
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
	
	// Calculate the interactions on the last element directly (using an N^2 approach)
	double directPot = 0.0;
	int nElements = mesh.elements.size();
	meshElement* targetElement = &mesh.elements[nElements-1];
	targetElement->potential = 0.0;
	for (int i=0; i<nElements-1; i++)
	{
		mesh.elements[i].directPotential(targetElement);
		mesh.elements[i].directVelocity(targetElement);
	}
	cout << "================" << endl;
	cout << "Direct potential at target element = " << targetElement->potential << endl;
	cout << "Direct velocity at target element = (" << targetElement->velocity[0] << "," << targetElement->velocity[1] << ")" << endl;
	cout << "================" << endl;
	// Clear target potential value for FMM calculation
	targetElement->clearValues();
	
	// Construct the FMM-tree for potential elements
	gettimeofday(&start, NULL);
	fmmTree2d tree(mesh, 6, 1);	// Make an fmmTree from the mesh (p=6)
	gettimeofday(&stop, NULL);
	cout << "Constructed the tree in: " << timeDiff(start, stop) << endl;

	// Start a global FMM timer
	gettimeofday(&start1, NULL);

	// Perform the multipole expansions on leaf boxes
	cout << "Performing multipole expansions:" << endl;
	gettimeofday(&start, NULL);
	tree.multipoleExpansion();	
	gettimeofday(&stop, NULL);	
	cout << "Completed multipole expansions in:" << timeDiff(start, stop) << endl;
	
	// Perform the upward pass
	cout << "Performing upward pass:" << endl;
	gettimeofday(&start, NULL);
	tree.upwardPass();	
	gettimeofday(&stop, NULL);
	cout << "Completed upward pass in:" << timeDiff(start, stop) << endl;
	
	// Perform the downward pass
	cout << "Doing cousin interactions:" << endl;
	gettimeofday(&start, NULL);
	tree.topBox.doCousinInteractions();	
	gettimeofday(&stop, NULL);
	cout << "Completed cousin interactions in:" << timeDiff(start, stop) << endl;

	cout << "Doing local translations and direct interactions:" << endl;
	gettimeofday(&start, NULL);
	tree.topBox.doLocalTranslations();	
	gettimeofday(&stop, NULL);
	cout << "Completed local interactions in:" << timeDiff(start, stop) << endl;

	// Print out the time to complete all FMM operations
	gettimeofday(&stop1, NULL);
	cout << "Completed all FMM operations in:" << timeDiff(start1, stop1) << endl;

	// Print out the potential calculated through the FMM
	cout << "================" << endl;
	cout << "FMM potential at target element = " << targetElement->potential << endl;
	cout << "FMM velocity at target element = (" << targetElement->velocity[0] << "," << targetElement->velocity[1] << ")" << endl;
	cout << "================" << endl;

	// Write the quad-tree to VTK format
	cout << "Writing FMM Tree object:" << endl;
	gettimeofday(&start, NULL);
	tree.writeVTK();	// write the tree to VTK (takes ages for large number of levels)
	gettimeofday(&stop, NULL);
	cout << "Wrote the FMM tree in:" << timeDiff(start, stop) << endl;

	return 0;
};

