// Class and operations for FMM Box

#include <iostream>

class pointElement2D
{
private:
double P0[3];
public:
complex valueAtPoint(double P[3]);
}

class elementField
{
private:
public:
int nElements;
}

class fmmTree
{
private:
int nLevels;
elementField * elements
public:
void buildFmmTree();
};

class fmmBox
{
private:
elementField * elements;
int treeLevel;
int levelIndex;
int boxDimension;
fmmBox * parentBox;
fmmBoxList * childBoxes;
fmmBoxList * neighbourBoxes;
public:
int nElements;
int boxCentre();
};


