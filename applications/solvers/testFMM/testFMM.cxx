#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;

#include "primitives.h"
#include "sources.h"
#include "mesh.h"
#include "fmm.h"

int main()
{
	// Read in points and faces from a polyMesh directory
	mesh2d mesh;
    mesh.read();
	
	// Construct the FMM-tree for potential elements
	fmmTree2d fmm(mesh);
	fmm.topBox.split();

	return 0;
}

