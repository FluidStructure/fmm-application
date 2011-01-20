/* Classes for points in 2 and 3 dimensions*/

#ifndef MESH2D_H_
#define MESH2D_H_

#include "primitives.h"
#include <list>

#include <iostream>
#include <fstream>
#include <string>

#include <stdlib.h>
//using std::cout;
//using std::endl;
//using std::list;
using namespace std;

class mesh2d
{
private:
	int nPoints;
	list<pnt2d> points;
	void removeComments(string& line);
	void getListLength(ifstream& fin);
	
	// A template function for reading values of arbitrary type
	template <class T>
	void readListLine(string& line, T* values, int n);
	
	void readPoints();
	void readElements();
public:
    void read();
};

#endif
