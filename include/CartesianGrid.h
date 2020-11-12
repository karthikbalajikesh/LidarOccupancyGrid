/*
This file contains the declarations of the Cartesian Grid Class.
*/

#ifndef CARTESIANGRID_H
#define CARTESIANGRID_H

#include<math.h>
#include<cmath>
#include<vector>
#include<utility>
#include"PolarGrid.h"
#include"CartesianNode.h"

// Macro Definitions
#define intPair std::pair<int,int>
using std::vector;

class CartesianGrid {
public:
	// Methods
	// 1. Default Constructor
	CartesianGrid(PolarGridRP* _Polar);

	// 2. Function to Construct Grid
	void ConstructGrid();


	// Attributes
	vector<vector<CartesianNode>> aGrid;
	// EndPoints
	intPair aBottomLeft;
	intPair aTopLeft;
	intPair aTopRight;
	intPair aBottomRight;
	int aXRows, aYCols;

protected:
	// Methods
	// 1. Method to construct a single Row
	void ConstructRow(const int& _x, const int& _index);

	// 2. Method to check if a cell lies in the blind spots
	bool WithinRange(const int& _xIndex, const int& _yIndex);

	// Attributes
	int aWidth, aHeight; // overall dimensions of grid in meters (6x6)
	int aResolutionX, aResolutionY; // width and height of every cartesianNode
	intPair aCenter;
	PolarGridRP* pPolarGrid;
};

#endif // !CARTESIANGRID_H
