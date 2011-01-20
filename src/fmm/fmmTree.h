#include "primitives.h"
#include "fmmBox.h"

class fmmTree2d
{
private:
	pnt2d treeCentre;
	double treeDimension;
	fmmBox2d * zeroBox;
	list<pnt2d> * targets;
	srcField2d * sources;
	list<complex> * targetPot;
	list<vec2d> * targetVel;
	list<vec2d> * targetAcc;
public:
	void addElements(elmts2d& sources);
	void buildTree();
	void upwardPass();
	void downwardPass();
	void calcPot();
	void calcVel();
	void calcAcc();
}
