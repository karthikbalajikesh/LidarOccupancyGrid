/*
This file contains declarations of the Node in the polar coordinate system.
Class Name: Polar Node
Purpose: Hold the occupancy and coordinates of a given cell
*/

#ifndef POLARNODE_H
#define POLARNODE_H

#include<vector>
#include<utility>
#include<cmath>

using std::vector;
using std::pair;

class PolarNode {

public:
	// Methods
	// 1. Default Constructor
	PolarNode();

	// 2. Constructor with range and angle (Range in y axis, and theta in x axis)
	PolarNode(const int& _range, const int& _theta);

	// 3. Function to update the Occupancy variable 
	void UpdateOccupancy(const int& _occ);

	// 4. Function to check if a coordinate is present in the node
	bool CheckPresence(const float& _Range, const float& _theta);

	// 5. Function to get the bounds of the node
	vector<int> GetBounds() const;

	// 6. Function to get the occupancy of PolarNode
	int GetOccupancy() const;



protected:

	// Attributes
	int aWidth; // 10 degrees
	int aHeight; // 30 mm
	int aOccupancy; // 0- not occupied, 1- unknown, 2- Occupied
	// Center 
	pair<int, int> aCenter;


};

#endif // !POLARNODE_H
