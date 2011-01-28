#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;

#include "mesh.h"
#include "fmm.h"

int main()
{
	// Read in points and faces from a polyMesh directory
	mesh2d mesh;
	mesh.read();
	mesh.writeVTK();
	
	// Construct the FMM-tree for potential elements
	fmmTree2d tree(mesh);
	tree.topBox.split();

	// Write the quad-tree to VTK format
	//tree.writeVTK();

	return 0;
}

