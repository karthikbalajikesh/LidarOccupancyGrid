/*
This file contains definitions of the CartesianGrid class
*/

#include<CartesianGrid.h>

/////////////////// Public ///////////////////////////////

/*
	1. Constructor with PolarGrid Input
*/
CartesianGrid::CartesianGrid(PolarGridRP* _polar) :
	aCenter({ 0,0 }), aHeight(6000), aWidth(6000),
	aResolutionX(20), aResolutionY(20), aXRows(300),
	aYCols(300), aBottomLeft({ -3000,3000 }),
	aBottomRight({ -3000,-3000 }), aTopLeft({ 3000,3000 }),
	aTopRight({ 3000,-3000 }), pPolarGrid(_polar) {

	ConstructGrid();

}

/*
			2. Construct Grid
Purpose: The function constructs the aGrid.
Inputs: None
Return Type: None
*/
void CartesianGrid::ConstructGrid() {
	// Create Empty Vector
	aGrid = vector<vector<CartesianNode>>(aXRows);
	// calculate the first X axis value
	int XCoordinate = aBottomLeft.first + (aResolutionX / 2);
	for (int row = 0; row < aXRows; row++) {
		ConstructRow(XCoordinate, row);
		XCoordinate += aResolutionX;
	}
}


/////////////////////////////// Private ///////////////////////////////////
/*
			1. ConstructRow
Purpose: The function constructs ta single row in aGrid.
Inputs: _x - Xcoordinate
		_index - Index of x
Return Type: None
*/
void CartesianGrid::ConstructRow(const int& _x, const int& _index) {
	// first reserve space
	aGrid[_index].reserve(aYCols);
	// calculate Y coordinate
	int YCoordinate = aBottomLeft.second - (aResolutionY / 2);

	for (int col = 0; col < aYCols; col++) {
		aGrid[_index].emplace_back(_x, YCoordinate);
		// Check if within Range
		if (WithinRange(_index, col)) {
			CartesianNode* current = &aGrid[_index][col];
			//set the required pointer
			current->SetPolarPointer(pPolarGrid->GetPolarPointer(current->aOriginDistance,
				current->aOriginAngle));
		}
		YCoordinate -= aResolutionY;
	}
}

/*
			2. Within Range
Purpose: Function returns true when not in blindspot.
Inputs: _xIndex - Row index
		_yIndex - Column Index
Return Type: Bool
*/
bool CartesianGrid::WithinRange(const int& _xIndex, const int& _yIndex) {
	// first we search if it is outside radius of 3m
	CartesianNode* node = &aGrid[_xIndex][_yIndex];
	if (node->aOriginDistance < 3000) {
		// next check if it is over 0.15 m
		if (node->aOriginDistance >= 150) {
			return true;
		}
	}
	return false;
}

