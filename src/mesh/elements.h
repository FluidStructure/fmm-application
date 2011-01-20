#ifndef ELEMENT_H_
#define ELEMENT_H_

#include "primitives.h"
#include <string>

#include <iostream>
#include <stdlib.h>
using std::cout;
using std::endl;

class element
{
public:
    string type;
	int nPoints;
	list<pnt2d*> pointList;
};

#endif
