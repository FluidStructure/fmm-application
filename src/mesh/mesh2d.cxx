#include "mesh2d.h"

// 2D mesh methods
void mesh2d::read()
{
	cout << "Reading in 2D mesh at time=0..." << endl;
	// Read in points and elements
	readPoints();
	readElements();
};

void mesh2d::readElements()
{
	string line, buf;
	int nValues[1];
	int values[3];
	int i;
	
	ifstream fin("0/elements");
	
	// Get the integer number of points in total
	getListLength(fin);
	
	// Read in all of the points
	while(getline(fin, line, ')')) 
	{ 
		// Get the number of points for this element
		readListLine(line, nValues, 1); 
		
		// Get the list of points
		readListLine(line, values, 3); 
	}
	
	fin.close()
};

void mesh2d::readPoints()
{
	string line, buf;
	double values[3];
	int i;
	
	ifstream fin("0/points");
	
	// Get the integer number of points in total
	getListLength(fin);
	
	// Read in all of the points
	while(getline(fin, line, ')')) 
	{ 
		readListLine(line, values, 3); 
	}
	
	fin.close()
};

void mesh2d::getListLength(ifstream& fin)
{
	string line;
	
	// Get the integer number of points in total
	getline(fin, line, '(');
	removeComments(line);
	nPoints = atoi(line.c_str());
	cout << nPoints << endl;
};

template <class T>
void mesh2d::readListLine(string& line, T* values, int n)
{
	string buf;
	int i;
	//double values[n];
	
	// Remove comments
	removeComments(line);
	
	// Remove opening brackets and newlines and replace with leading whitespaces
	while ( (i = line.find('(')) >= 0 ) { line.replace(i, 1, " "); }
	while ( (i = line.find('\n')) >= 0 ) { line.replace(i, 1, " "); }

	// Remove leading whitespaces (and get next whitespace location)
	while ( (i = line.find(' ')) == 0 ) { line.replace(i, 1, ""); }

	if (line.length() <= 0) { return; }

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

	for (i=0;i<n;i++) 
	{ 
		if (i == n-1)
			cout << values[i]; 
		else
			cout << values[i] << ", "; 
	}
	cout << endl;
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
