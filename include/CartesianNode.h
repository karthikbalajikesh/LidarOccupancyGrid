/*
This file contains declarations of the Cartesian Node Class.
Class Purpose: Hold Vehical frame cartesian coordinates and pointer
			   to corresponding PolarNode
*/

#ifndef CARTESIANNODE_H
#define CARTESIANNODE_H

#include<vector>
#include<utility>
#include<cmath>
#include<math.h>
#include"utils.h"
#include"PolarGrid.h"
#include"PolarNode.h"


// Macros Definitions
#define intPair std::pair<int,int>
#define PI 3.14159265359

class CartesianNode {
public:
	// Methods
	// 1. Default Constructor
	CartesianNode();

	// 2. Constructor with Coordinates
	CartesianNode(const int& _x, const int& _y);

	// 3. Function to get access to the Polar pointer
	PolarNode* GetPolarPointer() const;

	// 4. Function to set Polar Pointer
	void SetPolarPointer(PolarNode* _polar);

	// 5. Function to check if a given Coordinate lies in the cell
	bool CheckPresence(const int& _x, const int& _y) const;

	// Attributes
	float aOriginDistance;
	float aOriginAngle;


protected:
	// Methods
	void AssignAngle();

	// Attributes
	int aWidth, aHeight; // Width and Height of the cell
	PolarNode* pPolar;
	intPair aCenter;


};
#endif // !CARTESIANNODE_H


