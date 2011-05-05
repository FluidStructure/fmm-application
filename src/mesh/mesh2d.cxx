#include "mesh.h"

meshElement::meshElement( int nPoints, int *pIndex, vector<pnt2d>& allPoints )
{
	clearValues();
	
	pointsIndices.reserve(nPoints);
	points.reserve(nPoints);

	for (int i=0;i<nPoints;i++)
	{
		pointsIndices.push_back( pIndex[i] );
		points.push_back( &allPoints[pIndex[i]] );
		
		pointValues.push_back(0.0);  // Just a strength of 1.0 for now
	}
};

// Function returns the geometric center of the element points
pnt2d meshElement::collocationPoint()
{
	pnt2d p;
	int nPoints = 0;
	for (int i=0;i<points.size();i++)
	{
		p.co[0] += points[i]->co[0];
		p.co[1] += points[i]->co[1];
		nPoints++;
	}
	p.co[0] = p.co[0]/(double)nPoints;
	p.co[1] = p.co[1]/(double)nPoints;
	return p;
};

void meshElement::clearValues()
{
	potential = 0.0;
	velocity[0] = 0.0;
	velocity[1] = 0.0;
};

void meshElement::minMaxPoints( pnt2d& minPoint, pnt2d& maxPoint) const
{
	int i, j;
	for (i=0; i<points.size(); i++)
	{
		for (j=0; j<2;j++)
		{
			if (points[i]->co[j] < minPoint.co[j]) { minPoint.co[j] = points[i]->co[j]; continue; }
			if (points[i]->co[j] > maxPoint.co[j]) { maxPoint.co[j] = points[i]->co[j]; }
		}
	}
	//cout << minPoint.distToOrigin() << "  fdsa" << endl;
};

void meshElement::expandMultipole ( double zo[], complex<double> ak[], int& p ) const
{
	// Assuming this element is a point element using only first point
	double q = pointValues[0];
	complex<double> z( points[0]->co[0]-zo[0], points[0]->co[1]-zo[1] );
	
	ak[0] += q;
	for (int k=1; k<p; k++)	{ ak[k] += -1.0*q*pow(z,k)/(double)k; }
};

void meshElement::expandMultipole ( double zo[], double lims[], complex<double> ak[], int& p ) const
{
	// Assuming this element is a line-element with two points
	int i,k;
	
	// Expand the line-integral pow(z,k) term
	// (C = z1-zo) & (D = z2-z1)
	complex<double> C( points[0]->co[0]-zo[0], points[0]->co[1]-zo[1] );
	complex<double> D( points[1]->co[0]-points[0]->co[0], 
					    points[1]->co[1]-points[0]->co[1] );
	
	double q1 = pointValues[0];
	double q2 = pointValues[1];
	
	// Integral for the strength
	ak[0] += q1*(lims[1] - lims[0])
		  + (q2-q1)*0.5*(pow(lims[1],2) - pow(lims[0],2));
	
	// For efficiency we'll actually construct the binomial expansion 
	// from a Pascals triangle-type operation as we go
	complex<double> T;
	double K;
	double B[p]; double Btmp[p];
	B[0] = 1.0; B[1] = 1.0;   // Prime the triangle with the first elements
	for (k=1; k<p; k++)
	{
		// perform integral for each term of a polynomial expansion of the (z-zo)^k term
		K = (double)k;
		for (i=0;i<=k;i++) 
		{
			// Binomial coefficient for polynomial term
			T = B[i]*pow(C,i)*pow(D,k-i);
			// Zero-order part
			ak[k] += (-q1/((K+1)*K)) * T*( pow(lims[1],k-i+1) - pow(lims[0],k-i+1) );
			// First-order (gradient of strength part)
			ak[k] += (-1.0*(q2-q1)/((K+2)*K)) * T*( pow(lims[1],k-i+2) - pow(lims[0],k-i+2) );
		}
		
		// Calculate next row of Pascals triangle into temporary holder
		Btmp[0] = 1.0;
		for (i=1;i<=k;i++) { Btmp[i] = B[i-1] + B[i]; }
		Btmp[k+1] = 1.0;
		
		// Replace with values from temporary holder
		for (i=0;i<=k+1;i++) { B[i] = Btmp[i]; }  
	}
};


void meshElement::directPotential(meshElement* target)
{
	pnt2d CP = target->collocationPoint();

	if (points.size() == 1) 
	{
		target->potential += pointValues[0]*log( points[0]->distToPoint(&CP) );
	}
	else if (points.size() == 2)
	{
		target->potential += 0.0;
	}
};

void meshElement::directVelocity(meshElement* target)
{
	pnt2d CP = target->collocationPoint();

	double X = CP.co[0] - points[0]->co[0];
	double Y = CP.co[1] - points[0]->co[1];
	double Rsq = pow(X,2) + pow(Y,2);

	if (points.size() == 1) 
	{
		target->velocity[0] += pointValues[0]*Y/Rsq;
		target->velocity[1] += pointValues[0]*X/Rsq;
	}
	else if (points.size() == 2)
	{
		target->velocity[0] += 0.0;
		target->velocity[1] += 0.0;
	}
};


//-------------------------------
// 2D mesh methods

void mesh2d::read()
{
	cout << "Reading in 2D mesh from folder 0/..." << endl;
	// Read in points and elements
	readPoints();
	readElements();
	readValues();
};

void mesh2d::writeVTK()
{
	int i;
	
	//string fileName = "VTK/";
	//fileName = meshName + ".vtk";
	
	cout << "Writing 2d mesh to VTK unstructured grid format " << endl;
	ofstream fout(("VTK/"+meshName+"0000.vtk").c_str());
	//------------
	fout << "# vtk DataFile Version 2.0" << endl;
	fout << "OpenVOAM Unstructured Grid Mesh" << endl;
	fout << "ASCII" << endl;
	fout << "DATASET UNSTRUCTURED_GRID" << endl;
	//POINTS
	fout << "POINTS " << points.size() << " float" << endl; 
	for (i=0;i<points.size();i++)
	{
		fout << points[i].co[0] << " " << points[i].co[1] << " " << "0.0" << endl;
	}
	//CELLS
	int nData = 0;
	for (i=0; i<elements.size(); i++)
	{
		nData ++;
		for (int p=0; p<elements[i].pointsIndices.size(); p++) { nData++; }
	}
	fout << "CELLS " << elements.size() << " " << nData << endl;
	//
	for (i=0; i<elements.size(); i++)
	{
		fout << elements[i].pointsIndices.size();
		for (int p=0; p<elements[i].pointsIndices.size(); p++) { fout << " " << elements[i].pointsIndices[p]; }
		fout << endl;
	}
	//
	fout << "CELL_TYPES " << elements.size() << " " << endl;
	for (i=0; i<elements.size(); i++)
	{
		if (elements[i].pointsIndices.size() == 1) { fout << 1 << endl; }
		else if (elements[i].pointsIndices.size() == 2) { fout << 3 << endl; }
		else if (elements[i].pointsIndices.size() == 4) { fout << 8 << endl; }
		else { fout << 0 << endl; }
	}
	//------------
	fout.close();
};

void mesh2d::readValues()
{
	string line, buf;
	double values[4];	// First value is number of points, up to 3 points (triangles for now)
	int i,sz;
	
	ifstream fin("0/values");
	
	// Get the integer number of elements in total
	int nElements = getListLength(fin);

	// Read in all of the elements
	int e = 0;
	while(getline(fin, line, ')')) 
	{ 
		// Get the number of points for this element
		if ( readListLine(line, values, 4) )
		{
			// Add the read values to the elements list for the mesh
			sz =  elements[e].pointValues.size();
			for (i=1; i<sz+1; i++)
			{
				elements[e].pointValues[i-1] = values[i];
			}
			e++;
		}
	}
	fin.close();
};

void mesh2d::readElements()
{
	string line, buf;
	int values[4];	// First value is number of points, up to 3 points (triangles for now)
	int i;
	
	ifstream fin("0/elements");
	
	// Get the integer number of elements in total
	int nElements = getListLength(fin);
	elements.reserve(nElements);
	
	// Read in all of the elements
	while(getline(fin, line, ')')) 
	{ 
		// Get the number of points for this element
		if ( readListLine(line, values, 4) )
		{
			// Add the read values to the elements list for the mesh
			elements.push_back( meshElement( values[0], &values[1], this->points ) );	
		}
	}
	fin.close();
};

void mesh2d::readPoints()
{
	string line, buf;
	double values[3];
	int i;
	
	ifstream fin("0/points");
	
	// Get the integer number of points in total
    int nPoints = getListLength(fin);
	points.reserve(nPoints);

	// Read in all of the points
	while(getline(fin, line, ')')) 
	{ 
		if ( readListLine(line, values, 3) )
        {
			// Add the read values to the points list for the mesh
			points.push_back(pnt2d( values[0], values[1] ));
        }
	}
	fin.close();
};

int mesh2d::getListLength(ifstream& fin)
{
	string line;
	
	// Get the integer number of points in total
	getline(fin, line, '(');
	removeComments(line);
	return atoi(line.c_str());
};

template <class T>
int mesh2d::readListLine(string& line, T* values, int n)
{
	string buf;
	int i;
	
	// Remove comments
	removeComments(line);
	
	// Remove opening brackets and newlines and replace with leading whitespaces
	while ( (i = line.find('(')) >= 0 ) { line.replace(i, 1, " "); }
	while ( (i = line.find('\n')) >= 0 ) { line.replace(i, 1, " "); }

	// Remove leading whitespaces (and get next whitespace location)
	while ( (i = line.find(' ')) == 0 ) { line.replace(i, 1, ""); }

	if (line.length() <= 0) { return 0; }

	for (int c=0; c<n; c++)
	{
		// Remove leading whitespaces (and get next whitespace location)
		while ( (i = line.find(' ')) == 0 ) { line.replace(i, 1, ""); }
		
		// Extract values if the length of the string is greater than 0
		buf = line.substr(0, i);
		line.replace(0, i, "");
		if (buf.length() > 0)
			values[c] = strtod( buf.c_str(), NULL );
		else
			values[c] = 0;
	}
    return 1;
};

void mesh2d::removeComments(string& str)
{
	int s, f, cpos1, cpos2;
	string end;
	
	// Do this until no more comment characters are found
	s = 0;
	while (s >= 0)
	{
		// Find first occurance of comment character
		cpos1 = str.find("/*", s);
		cpos2 = str.find("//", s);
		if ((cpos1 >= 0) and (cpos2 >= 0))
			s = min(cpos1, cpos2);
		else if ((cpos1 >= 0) or (cpos2 >= 0)) 
			s = max(cpos1, cpos2); 
		else
			{ s = -1; return; }
		
		// Get ending character based on type of comment
		if ((s >= 0) and (s == cpos1))
			end = "*/";
		else if ((s >= 0) and (s == cpos2))
			end = "\n";
		
		// Find where the ending character is
		f = str.find(end, s);
		if (end == "*/")
			f += 2;
		else if (end == "\n")
			f -= 1;
		
		// Erase content between start and finish points
		// If finish point is not found then delete rest of string
		if (f >= 0)
			str.erase(s, f);
		else
			str.erase(s);
	}
	return;
};
