/*
File contains function definitions of PolarGrid and PolarGridRP
*/

#include"PolarGrid.h"

// PolarGrid

PolarGrid::PolarGrid(const int& _sizeX, const int& _sizeY) :
	aXRows(_sizeX), aYCols(_sizeY),
	aResolutionX(30), aResolutionTheta(10) {

}

// PolarGridRP

PolarGridRP::PolarGridRP() : PolarGrid(95, 36) {
	CreateGrid();
}

//////////////////////////////////// Public ///////////////////////////////
/*
		2. CreateGrid
Function: Creates the complete PolarGrid
Inputs: None
Return type: None
*/
void PolarGridRP::CreateGrid() {
	aGrid = vector<vector<PolarNode>>(aXRows);
	int Range = 165;
	for (int index = 0; index < aXRows; index++) {
		ConstructRow(Range, index);
		Range += aResolutionX;
	}
}


/*
		3. FindNodeIndex(Binary Search)
Function: Creates the complete PolarGrid
Inputs: _range - Range in mm
		_angle - Angle in degree
Return type: pair<int,int> in the form <x_index, y_index>
*/
intPair PolarGridRP::FindNodeIndex(const float& _range, const float& _angle) {
	// convert angle and range to integers
	int Range = (int)std::round(_range);
	int Angle = (int)std::round(_angle);
	// Wrap range to within 3000
	if (Range == 3000) {
		Range = 2999;
	}
	// Wrap Angle to within 360 degree
	if (Angle == 360) {
		Angle = 359;
	}
	// Define Index Bounds
	int MaxR = this->aXRows, MinR = 0;
	int MaxTheta = this->aYCols, MinTheta = 0;
	// Calculate the Medians
	int IndexR = (MaxR + MinR) / 2;
	int IndexTheta = (MaxTheta + MinTheta) / 2;
	// Get the current Node
	PolarNode* CurrentNode = &aGrid[IndexR][IndexTheta];

	while (!CurrentNode->CheckPresence(_range, _angle)) {
		// Get the bounds of the Current node
		vector<int> bounds = CurrentNode->GetBounds();
		int UpperR = bounds[0], LowerR = bounds[1];
		int UpperAngle = bounds[2], LowerAngle = bounds[3];


		// 1. Check for the lower bound of the current node vs desired point
		if (LowerR <= Range) {
			MinR = IndexR;
		}
		if (UpperR <= Range) {
			MinR = IndexR + 1;
		}
		if (LowerAngle <= Angle) {
			MinTheta = IndexTheta;
		}
		if (UpperAngle <= Angle) {
			MinTheta = IndexTheta + 1;
		}

		// 2. Check for the upper bound of the currentnode vs desired point
		if (UpperR > Range) {
			MaxR = IndexR;
		}
		if (LowerR > Range) {
			MaxR = IndexR - 1;
		}
		if (UpperAngle > Angle) {
			MaxTheta = IndexTheta;
		}
		if (LowerAngle > Angle) {
			MaxTheta = IndexTheta - 1;
		}

		// Update the Node
		IndexTheta = (MinTheta + MaxTheta) / 2;
		IndexR = (MinR + MaxR) / 2;
		CurrentNode = &aGrid[IndexR][IndexTheta];
	}
	return { IndexR, IndexTheta };
}


/*
		4. Reset
Function: Removes past occupancy information (THREADED)
Inputs: None
Return type: None
*/
void PolarGridRP::Reset() {
	// Create thread Pool
	std::vector<std::thread> Pool;
	// Go through every column
	for (int column = 0; column < aYCols; column++) {
		// Create thread 
		std::thread th(&PolarGridRP::ResetColumn, this, column);
		Pool.push_back(std::move(th));
	}

	// Join the threads
	for (std::thread& th : Pool) {
		if (th.joinable()) {
			th.join();
		}
	}
}


/*
		5. Transform  Coordinates
Function: Converts from Lidar Cood frame to Global frame
Inputs: measurements - A float pair of the form (R, theta)
		State - State of the vehicle in (x,y,theta_pose)
Return type: vector<float> Edited copy of measurements

Equation : Theta_effective = Theta_pose from Localization
									- THeta_lidar from Measurement + 360
									and wrapped to 360
*/
vector<floatPair> PolarGridRP::TransformCoordinates(vector<floatPair> measurements,
	const vector<long long>& _State) const {
	float ThetaPose = _State[2];
	for (int index = 0; index < measurements.size(); index++) {
		float ThetaLidar = measurements[index].second;
		measurements[index].second = ThetaPose - ThetaLidar + 360.0;
		if (measurements[index].second >= 360.00001) {
			// wrap the angle
			measurements[index].second -= 360;
		}
	}
	// return the copy
	return measurements;
}

/*
		6. UpdateOccupancy
Function: updates the current occupied and unknown cells
Inputs: measurements : vector<floatPair> Reference, vehicle frame polar coordinates
Return type: None
*/
void PolarGridRP::UpdateOccupancy(const vector<floatPair>& measurements) {
	// Loop through the measurements
	for (int reading = 0; reading < measurements.size(); reading++) {
		// find index 
		intPair Indices = this->FindNodeIndex(measurements[reading].first,
			measurements[reading].second);
		// Update that node as an obstacle and the next node as well
		aGrid[Indices.first][Indices.second].UpdateOccupancy(2);
		if (Indices.first + 1 < this->aXRows) {
			aGrid[Indices.first + 1][Indices.second].UpdateOccupancy(2);
		}
		// Make the other vertices above as unknown if they are not classified as obstacle
		for (int index = Indices.first + 2; index < aXRows; index++) {
			if (aGrid[index][Indices.second].GetOccupancy() != 2) {
				aGrid[index][Indices.second].UpdateOccupancy(1);
			}
		}

	}
}


/*
		6. Update
Function: Calls the Reset, Transform and Update in the required order
Inputs: measurements : None
Return type: None
*/
void PolarGridRP::Update(vector<floatPair>& measurements, vector<long long> _state) {
	std::thread Resetter(&PolarGridRP::Reset, this);
	// Transform coordinates in current thread
	vector<floatPair> newMeasurements = TransformCoordinates(measurements, _state);
	Resetter.join();
	// Now update the Polar Occupancy
	UpdateOccupancy(newMeasurements);
}


/*
		7. GetPolarPointer
Function: Returns the polarNode pointer based on range and theta
Inputs: measurements : _range - Range in mm (float)
					   _angle - angle in degrees(float)
Return type: None
*/
PolarNode* PolarGridRP::GetPolarPointer(const float& _range,
	const float& _angle) {
	// Binary Search for Polar Node
	intPair Indices = FindNodeIndex(_range, _angle);

	// Return Address
	return &aGrid[Indices.first][Indices.second];
}


//////////////////////////////////// Private ////////////////////////////////
/*
		1. ConstructRow
Function: Optimized Creation of the vector
Inputs: None
Return type: None
*/
void PolarGridRP::ConstructRow(const int& _Range, const int& _index) {
	// First reserve space
	aGrid[_index].reserve(aYCols);
	// get the center theta 
	int Theta = 5;
	for (int column = 0; column < aYCols; column++) {
		aGrid[_index].emplace_back(_Range, Theta);
		Theta += aResolutionTheta;
	}
}


/*
		2. ResetColumn
Function: Resets one column of a Grid
Inputs: None
Return type: None
*/
void PolarGridRP::ResetColumn(int _index) {

	for (int row = 0; row < aXRows; row++) {
		// Update occupancy of the Node to 0
		aGrid[row][_index].UpdateOccupancy(0);
	}
}

