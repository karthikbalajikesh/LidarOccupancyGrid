/*
This file contains the Declarations of the class PolarGrid.
PolarGridRP is the version for RPLIDAR.
*/

#ifndef POLARGRID_H
#define POLARGRID_H

#include<vector>
#include<cmath>
#include<utility>
#include<thread>
#include"utils.h"
#include"PolarNode.h"
#include"Eigen/Dense"

using std::vector;
using std::pair;

#define intPair pair<int, int>
#define floatPair pair<float,float>

/*
Class 1 - PolarGrid
Purpose: Abstract class that contains declarations of necessary members
		 for a polar grid
*/
class PolarGrid {
public:
	// Constructor with Required Vars
	PolarGrid(const int& _sizeX, const int& _sizeY);

	// 2. Function that creates the Grid 
	virtual void CreateGrid() = 0;

	// 3. Binary Search Function
	virtual intPair FindNodeIndex(const float& _range,
		const float& _angle) = 0;

	// 4. Reset Function 
	virtual void Reset() = 0;

	// 5. Function that converts between sensor coordinates to GLobal coordinates
	virtual vector<floatPair> TransformCoordinates(vector<floatPair> measurements,
		const vector<long long>& _State) const = 0;

	// 6. Function that updates the occupancy based on Lidar Information
	virtual void UpdateOccupancy(const vector<floatPair>& measurements) = 0;

	// 7. The main update Function
	virtual void Update(vector<floatPair>& measurements, vector<long long> _state) = 0;



protected:
	// Attributes
	vector<vector<PolarNode>> aGrid;
	int aXRows, aYCols;
	int aResolutionX;
	int aResolutionTheta;

};


/*
Class 2 - PolarGridRP
Purpose: The class implements the PolarGrid Interface with grid structure
		 suitable for RPLidar A1.
*/
class PolarGridRP :public PolarGrid {
public:
	// Constructor with Required Vars
	PolarGridRP();

	// 2. Function that creates the Grid 
	void CreateGrid() override;

	// 3. Binary Search Function
	intPair FindNodeIndex(const float& _range,
		const float& _angle) override;

	// 4. Reset Function 
	void Reset() override;

	// 5. Function that converts between sensor coordinates to Vehicle coordinates
	vector<floatPair> TransformCoordinates(vector<floatPair> measurements,
		const vector<long long>& _State) const override;

	// 6. Function that updates the occupancy based on Lidar Information
	void UpdateOccupancy(const vector<floatPair>& measurements) override;

	// 7. The main update Function
	void Update(vector<floatPair>& measurements, vector<long long> _state) override;

	// 8. Function to get PolarNode pointer given a coordinate
	PolarNode* GetPolarPointer(const float& _range, const float& _angle);


protected:
	// Members
	// 1. Function to construct a Row
	void ConstructRow(const int& _Range, const int& _index);

	// 2. Function to Reset a column to free
	void ResetColumn(int _index);
};


#endif // !POLARGRID_H
