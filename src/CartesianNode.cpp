/*
This file contains the function definitions for the CartesianNode class
present in CartesianNode.h file.
*/

#include"CartesianNode.h"

using std::pow;
using std::sqrt;

///////////////////////////Public/////////////////////////////////
/*
		1. Default Constructor
Purpose: Assigns the Cartesian Node with Center origin
Inputs: None
Return Type: None
*/
CartesianNode::CartesianNode() :aWidth(20), aHeight(20),
pPolar(NULL), aCenter({ 0,0 }), aOriginAngle(0),
aOriginDistance(0) {}


/*
		2. Constructor with coordinates
Purpose: Assigns the Cartesian Node with arguments as center
Inputs: _x - x coordinate in mm
		_y - y coordinate in mm
Return Type: None
*/
CartesianNode::CartesianNode(const int& _x, const int& _y) :
	aCenter({ _x,_y }), aWidth(20), aHeight(20), pPolar(NULL) {
	// calculate distance from origin
	aOriginDistance = sqrt(pow(aCenter.first, 2)
		+ pow(aCenter.second, 2));
	// Calculate Angle from origin
	AssignAngle();

}


/*
		3. GetPolarPointer
Purpose: Function returns the polar pointer associated with cell
Inputs: None
Return Type: PolarNode* pointer
*/
PolarNode* CartesianNode::GetPolarPointer() const {
	return pPolar;
}


/*
		4. SetPolarPointer
Purpose: Function sets the polar pointer associated with cell
Inputs: PolarNode* pointer
Return Type: None
*/
void CartesianNode::SetPolarPointer(PolarNode* _polar) {
	pPolar = _polar;
}

/*
		5. CheckPresence
Purpose: Function returns true if given coordinate is within cell
Inputs: -x - x coordinate in mm
		_y - y coordinate in mm
Return Type: None
*/
bool CartesianNode::CheckPresence(const int& _x, const int& _y) const {

	if (_x < (aCenter.first + (aHeight / 2)) &&
		_x >= (aCenter.first - (aHeight / 2))) {
		if (_y > (aCenter.second - (aWidth / 2)) &&
			(_y <= (aCenter.second + (aWidth / 2)))) {
			return true;
		}
	}
	return false;
}


//////////////////////////////Private ////////////////////////////
/*
		1. Assign Angle
Purpose:Calculates the angle from the origin in vehicle sense
Inputs: None
Return Type: None
*/
void CartesianNode::AssignAngle() {

	float theta = (atan((float)aCenter.second / (float)aCenter.first)) * (180 / PI);
	if (aCenter.first >= 0 && aCenter.second >= 0) {
		aOriginAngle = theta;
	}
	else if (aCenter.first < 0 && aCenter.second >= 0) {
		aOriginAngle = 180 + theta;
	}
	else if (aCenter.first < 0 && aCenter.second < 0) {
		aOriginAngle = 180 + theta;
	}
	else if (aCenter.first >= 0 && aCenter.second < 0) {
		aOriginAngle = 360 + theta;
	}

}