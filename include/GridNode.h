/*
This header contains the declarations of the 2x2m Grid node, which is composed of
2x2 cm cells.

Purpose:
	Hold the coordinates and occupancy of region
	Search Function to identify coordinate
*/

#ifndef GRIDNODE_H
#define GRIDNODE_H

#include<utility>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<iostream>
#include<thread>
#include"Node.h"

// Macros Definition
#define longMap std::unordered_map<std::pair<long long, long long>>
#define indexPair std::pair<int, int>


/*
Class 2 - MacroNode
" It is the 2x2 m GridNode object that implements the abstract class GridNode"
*/

class MacroNode {
public:
	// Methods
	// 1. Default Constructor with origin as center
	MacroNode();

	// 2. Constructor with center coordinate
	MacroNode(const long long& _x, const long long& _y);

	// 3. Function that returns the center coordinates
	longPair GetCenterCoordinates() const;

	// 4. Function that checks if a coordinate is present within Grid
	bool InGrid(const long long& _x, const long long& _y) const;

	// 5. Binary Search function
	indexPair GetIndex(const long long& _x,
		const long long& _y);

	// Attributes
	// 1. Grid 
	vector<vector<Node2x2>> aGrid;

	// 2. Neighbor pointers
	MacroNode* pNorth;
	MacroNode* pEast;
	MacroNode* pSouth;
	MacroNode* pWest;

	// 3. Vector sizes
	int aSizeX;
	int aSizeY;

	// 4. End points
	longPair aBottomLeft;
	longPair aBottomRight;
	longPair aTopRight;
	longPair aTopLeft;

	// 5. Dimensions
	int aHeight; // Height of the Grid - 2 m
	int aWidth;  // Width of the Grid - 2 m

protected:
	// Methods
	// 1. Method to construct the Grid
	void ConstructGrid();

	// 2. Method to define the endpoints of the grid
	void DefineEndPoints();

	// 3. Method to create a row of Nodes
	void CreateRow(const int& _xIndex, const long long& _xCoordinate);

	//4. Method to update lower limits in Binary Search
	void UpdateLowerBound(const long long& _x,
		const long long& _y, const vector<long long>& bounds, int& minX, int& minY,
		int& indexX, int& indexY) const;

	//4. Method to update upper limits in Binary Search
	void UpdateUpperBound(const long long& _x, const long long& _y,
		const vector<long long>& bounds, int& maxX, int& maxY,
		int& indexX, int& indexY) const;


	// Attributes
	int aResolutionX; // Resolution of cell - 2 cm x 2 cm
	int aResolutionY;
	longPair aCenterCoordinate;

};

#endif 
