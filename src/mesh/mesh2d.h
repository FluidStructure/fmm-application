/* Classes for points in 2 and 3 dimensions*/

#ifndef MESH2D_H_
#define MESH2D_H_

#include "primitives.h"
#include <vector>

#include <iostream>
#include <fstream>
#include <string>

#include <stdlib.h>
using namespace std;

class mesh2d
{
private:
    // Containers for all element types
    //list<pointVortex> pointVortices;
    //list<lineVortex> lineVortices;
    
	// A template function for reading values of arbitrary type
	template <class T>
	int readListLine(string& line, T* values, int n);
	
	void removeComments(string& line);
	int getListLength(ifstream& fin);

	void readPoints();
	void readElements();
public:
    void read();

    int nPoints;
	vector<pnt2d> points;
};

#endif
