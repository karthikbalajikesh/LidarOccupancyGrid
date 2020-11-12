/*
This file contains the polar node definitions.
*/
#include"PolarNode.h"


//////////////////////   Public //////////////////////////////////////

/*
		1. Default Constructor
*/
PolarNode::PolarNode() : aCenter({ 165,5 }), aWidth(10), aHeight(30),
aOccupancy(0) {

}

/*
		2. Overloaded Constructor
Function: Accepts the center coordinate and forms the polarNode with default
			occupancy of 0.
Inputs: _range - Integer specifying the center range in mm
		_theta - Integer specifying the center bearing angle in degree
*/
PolarNode::PolarNode(const int& _range, const int& _theta) :
	aCenter({ _range,_theta }), aWidth(10), aHeight(30), aOccupancy(0) {

}


/*
		3. Update Occupancy
Function: Updates the class variable occupancy based on input
Inputs: _occ - Occupancy value to be updated (type int)
Return Type: Void
*/
void PolarNode::UpdateOccupancy(const int& _occ) {
	aOccupancy = _occ;
}


/*
		4. Check Presence
Function: returns true if given coordinate lies in the variable
Inputs: _range - Range in mm
		_theta - Theta in degree
Return type: bool
*/
bool PolarNode::CheckPresence(const float& _range, const float& _theta) {
	// Round and convert to integer
	int Range = (int)std::round(_range);
	int Theta = (int)std::round(_theta);

	// wrap angle
	if (Theta == 360) {
		Theta = 359;
	}

	// wrap Range
	if (Range == 3000) {
		Range = 2999;
	}

	if ((Theta >= aCenter.second - (aWidth / 2)) && (Theta < aCenter.second + (aWidth / 2))) {
		// If theta lies within the limits
		if ((Range >= aCenter.first - (aHeight / 2)) && (Range < aCenter.first + (aHeight / 2))) {
			// Within Range boundaries
			return true;
		}
	}

	return false;
}


/*
		5. GetBounds()
Function: Returns vector in form {R_max, R_min, Theta_max, Theta_min}
Inputs: None
Return type: vector<int>
*/
std::vector<int> PolarNode::GetBounds() const {
	// Create empty vector
	std::vector<int> Bounds(4, 0);
	Bounds[0] = aCenter.first + (aHeight / 2); // Rmax
	Bounds[1] = aCenter.first - (aHeight / 2); // Rmin
	Bounds[2] = aCenter.second + (aWidth / 2); // ThetaMax
	Bounds[3] = aCenter.second - (aWidth / 2); // ThetaMin

	return Bounds;
}


/*
		5. GetOccupancy
Function: Returns 0 if free, 1 if unknown and 2 if occupied
Inputs: None
Return type: int
*/
int PolarNode::GetOccupancy() const {
	return aOccupancy;
}