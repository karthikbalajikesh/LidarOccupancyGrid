/*
The file contains function declarations of Macro Node
*/


#include"GridNode.h"


/*
		   1. Default Constructor

Function: Assigns sizes, pointers, end points and the grid
*/
MacroNode::MacroNode() : aResolutionX(20), aResolutionY(20),
aCenterCoordinate({ 0,0 }), aHeight(2000), aWidth(2000), pNorth(NULL),
pSouth(NULL), pEast(NULL), pWest(NULL) {
	// Define the vector sizes
	aSizeX = aHeight / aResolutionX;
	aSizeY = aWidth / aResolutionY;

	// Define the end points
	DefineEndPoints();

	// Construct the Grid
	ConstructGrid();


}


/*
			2. Overloaded Constructor

Function: Assigns sizes, pointers, end points and the grid
Input: long long _x and long long _y : x and y coordinates of center in mm
*/
MacroNode::MacroNode(const long long& _x, const long long& _y) :
	aResolutionX(20), aResolutionY(20),
	aCenterCoordinate({ _x,_y }), aHeight(2000), aWidth(2000), pNorth(NULL),
	pSouth(NULL), pEast(NULL), pWest(NULL) {
	// Define the vector sizes
	aSizeX = aHeight / aResolutionX;
	aSizeY = aWidth / aResolutionY;

	// Define the end points
	DefineEndPoints();

	// Construct the Grid
	ConstructGrid();
}


/*
			3. GetCenterCoordinates

Function: returns a pair of long long variables (x,y) depicting center
Input: None
*/
longPair MacroNode::GetCenterCoordinates() const {

	return this->aCenterCoordinate;
}


/*
			4. InGrid

Function: Checks if a point is present in the MacroNode
Input: long long _x and long long _y : x and y coordinates in mm
*/
bool MacroNode::InGrid(const long long& _x, const long long& _y) const {

	if (_x < aTopLeft.first && _x >= aBottomLeft.first) {
		if (_y <= aBottomLeft.second && _y > aBottomRight.second) {
			return true;
		}
	}
	return false;
}


/*
			5. GetIndex

Function: Return index of cell where point belongs
Input: long long _x and long long _y : x and y coordinates in mm
*/
indexPair MacroNode::GetIndex(const long long& _x,
	const long long& _y) {

	// Define limit and median variables
	int maxX = aSizeX - 1;
	int minX = 0;
	int maxY = aSizeY - 1;
	int minY = 0;
	int IndexX = (int)(maxX + minX) / 2;
	int IndexY = (int)(maxY + minY) / 2;
	// Define the 
	Node* CurrentNode = &this->aGrid[IndexX][IndexY];
	while (!CurrentNode->CheckPresence(_x, _y)) {
		// loop enters when current node is not desired node
		// Get the boundaries of the current Node
		std::vector<long long> bounds = CurrentNode->GetBounds();

		// Update the lower and upper bounds
		UpdateLowerBound(_x, _y, bounds, minX, minY, IndexX, IndexY);
		UpdateUpperBound(_x, _y, bounds, maxX, maxY, IndexX, IndexY);

		// Update the Current Node
		IndexX = (minX + maxX) / 2;
		IndexY = (minY + maxY) / 2;
		CurrentNode = &this->aGrid[IndexX][IndexY];

	}
	// Loop exits when cell with coordinate is found
	return { IndexX,IndexY };
}

//////////////////////  Private  //////////////////////////////////////


/*
			1. Construct Grid

Function: Creates grid using a pool of threads
Input: None
*/
void MacroNode::ConstructGrid() {
	aGrid = vector<vector<Node2x2>>(aSizeX);
	// create x_coordinate value
	long long xCoordinate = this->aBottomLeft.first + (aResolutionX / 2);

	// create a pool of threads in a for loop and execute the constuction of Grid
	for (int index = 0; index < this->aSizeX; index++) {
		// call the construct Row function
		CreateRow(index, xCoordinate);

		// Get next x coordinate
		xCoordinate += this->aResolutionX;
	}

}

/*
			2. Define Endpoints

Function: Defines the value of the end point variables
Input: None

*/
void MacroNode::DefineEndPoints() {
	aBottomLeft = { aCenterCoordinate.first - (aHeight / 2),
				aCenterCoordinate.second + (aWidth / 2) };
	aBottomRight = { aBottomLeft.first,
				aCenterCoordinate.second - (aWidth / 2) };
	aTopLeft = { aBottomLeft.first + aHeight,
				aBottomLeft.second };
	aTopRight = { aTopLeft.first,
				aBottomRight.second };
}

/*
			3. CreateRow

Function: Creates a row of Nodes based on the inputs
Input: 1. _xIndex - Row index number
	   2. _xCoordinate - x coordinate value in mm
*/
void MacroNode::CreateRow(const int& _xIndex,
	const long long& _xCoordinate) {

	// Reserve space for the columns of the row
	this->aGrid[_xIndex].reserve(this->aSizeY);

	// Mock y coordinate value
	long long _yCoordinate = this->aBottomLeft.second - (aResolutionY / 2);
	// Loop through and add the Nodes
	for (int index = 0; index < this->aSizeY; index++) {
		// call node constructor
		this->aGrid[_xIndex].emplace_back(_xCoordinate, _yCoordinate);

		// Subtract resolution to get next Y value(Right negative)
		_yCoordinate -= this->aResolutionY;
	}

}

/*
			4. UpdateLowerBound

Function: Function edits the lower bounds of x and y indices
Input: 1. _x - x coordinate in mm
	   2. _y - y coordinate in mm
	   3. vector<long long> bounds - Boundaries
	   4. minX - Minimum index for X
	   5. minY - Minimum index for Y
	   6. indexX - Current Median Index for X
	   7. indexY - Current Median Index for y
*/
void MacroNode::UpdateLowerBound(const long long& _x, const long long& _y,
	const vector<long long>& bounds, int& minX, int& minY,
	int& indexX, int& indexY) const {

	// add boundaries as variables
	long long upperX = bounds[0], lowerX = bounds[1];
	long long upperY = bounds[2], lowerY = bounds[3];

	if (lowerX <= _x) {
		// means check in north
		minX = indexX;
	}
	if (upperX <= _x) {
		minX = indexX + 1;
	}
	if (_y <= upperY) {
		// Due to coordinate system
		minY = indexY;
	}
	if (_y <= lowerY) {
		minY = indexY + 1;
	}
}

/*
			5. UpdateUpperBound

Function: Function edits the upper bounds of x and y indices
Input: 1. _x - x coordinate in mm
	   2. _y - y coordinate in mm
	   3. vector<long long> bounds - Boundaries
	   4. maxX - Maximum index for X
	   5. maxY - Maximum index for Y
	   6. indexX - Current Median Index for X
	   7. indexY - Current Median Index for y
*/
void MacroNode::UpdateUpperBound(const long long& _x, const long long& _y,
	const vector<long long>& bounds, int& maxX, int& maxY,
	int& indexX, int& indexY) const {

	// add boundaries as variables
	long long upperX = bounds[0], lowerX = bounds[1];
	long long upperY = bounds[2], lowerY = bounds[3];

	// check for the Upper bounds
	if (upperX > _x) {
		maxX = indexX;
	}
	if (lowerX > _x) {
		maxX = indexX - 1;
	}
	if (_y > lowerY) {
		maxY = indexY;
	}
	if (_y > upperY) {
		maxY = indexY - 1;
	}
}