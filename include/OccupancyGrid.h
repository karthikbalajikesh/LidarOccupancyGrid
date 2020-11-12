/*
This file contains declarations for the Occupancy Grid Class.
*/

#ifndef OCCUPANCYGRID_H
#define OCCUPANCYGRID_H

#include<vector>
#include<utility>
#include<math.h>
#include<cmath>
#include<Eigen/Dense>
#include"utils.h"
#include"Stitcher.h"
#include"CartesianGrid.h"
#include"PolarGrid.h"
#include"GridNode.h"


/*
Class Name: Occupancy Grid
Purpose: Interfaces the other Grid structures
*/
class OccupancyGrid {
public:
	// Methods
	// Default Constructor
	OccupancyGrid(Localization* _SE);

	// 2. Function that finds the location of a point in grid
	Point FindPoint(const long long& _x, const long long& _y);

	// 3. Major Update Function
	void Update(const vector<floatPair>& _measurements);

	// 4. Destructor
	~OccupancyGrid();



protected:
	// Methods
	// 1. Function to Narrow the x axis difference
	void VerticalScan(const long long& _x, MacroNode*& _Location);
	// 2. Function to Narrow y axis
	void HorizontalScan(const long long& _y, MacroNode*& _Location);
	// 3. Function to Update the PolarGrid
	void UpdatePolarGrid(vector<floatPair> _measurements);
	// 4. Function to Update Vehicle and BottomLeft Position
	void UpdatePosition();
	// 5. Function to find the index of Cartesian Grid in MacroNode
	Point FindMacroIndex(int _xIndex, int _yIndex);
	// 6. Function to get Log odds Measurement
	float InverseSensorModel(const int& _xIndex, const int& _yIndex);
	// 7. Function to Update Log odds with Point
	void UpdateLogOdds(const Point& _Node, const float& _LogOdds);
	// 8. Function to Update 3 Rows from given index
	void UpdateRowOccupancy(int _StartIndex);
	// 9. Function to create the initial Grid of 20x20m
	void CreateInitialGrid();
	// Attributes
	Stitcher Tailor;
	MacroNode* pHead;
	PolarGridRP aGridPolar;
	CartesianGrid aGridCart;
	Localization* pStateEstimator;
	Point aVehicleLocation;
	Point aBottomLeftLocation;
	int aFirstUpdate;
};

#endif // !OCCUPANCYGRID_H


