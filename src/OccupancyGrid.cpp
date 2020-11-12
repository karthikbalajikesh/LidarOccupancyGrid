/*
This file contains definitions of the Occupancy grid class.
*/

#include"OccupancyGrid.h"

//////////////////////////////////  Public /////////////////////////////////////
/*
		1. Default Constructor
Purpose: Allocates the required grids
Inputs: _SE - Pointer to localization class
Return Type: None
*/
OccupancyGrid::OccupancyGrid(Localization* _SE) :pStateEstimator(_SE),
aGridPolar(PolarGridRP()), aGridCart(CartesianGrid(&aGridPolar)),
aVehicleLocation(Point()), aBottomLeftLocation(Point()), Tailor(Stitcher()),
aFirstUpdate(0) {

	// Create the head node and add to the stitcher
	pHead = new MacroNode();
	Tailor.AddMacroNode(pHead);
	aVehicleLocation.pLocation = pHead;
	aBottomLeftLocation.pLocation = pHead;
	this->CreateInitialGrid();

}

/*
		2. FindPoint
Purpose: Finds the MacroNode and index for a coordinate
Inputs: _x - x coordinate
		_y - y coordinate in mm
Return Type: Point class object
*/
Point OccupancyGrid::FindPoint(const long long& _x, const long long& _y) {
	// We will start checking about current location
	MacroNode* location = aVehicleLocation.pLocation;

	// check if coordinate is present in the location MacroNode
	while (!location->InGrid(_x, _y)) {
		// This implies current MacroNode does not hold coordinate
		// If x axis is aligned, call the Y axis
		if (_x < (location->aTopLeft.first) &&
			_x >= (location->aBottomLeft.first)) {
			// This implies we have found x coordinate
			// Search for Y coordinate
			HorizontalScan(_y, location);
		}
		else {
			// This implies we still havent reached x coordinate
			VerticalScan(_x, location);
		}

	}

	Point Result;
	Result.pLocation = location;
	intPair Indices = location->GetIndex(_x, _y);
	Result.aXIndex = Indices.first;
	Result.aYIndex = Indices.second;

	return Result;

}

/*
		3. Update
Purpose: Update function called at every lidar update
Inputs: _measurements - Lidar readings
Return Type: None
*/
void OccupancyGrid::Update(const vector<floatPair>& _measurements) {
	// Update the Polargrid in a thread
	std::thread UpdateThread(&OccupancyGrid::UpdatePolarGrid, this, _measurements);

	// Meanwhile Update Current Location
	UpdatePosition();

	// Join the thread
	if (UpdateThread.joinable()) {
		UpdateThread.join();
	}


	// Finally Update the Grid
	// Loop through in steps of three
	vector<std::thread> Pool;
	for (int row = 0; row < aGridCart.aGrid.size(); row = row + 3) {
		std::thread th(&OccupancyGrid::UpdateRowOccupancy, this, row);
		Pool.push_back(std::move(th));
	}
	// Join threeads
	for (std::thread& th : Pool) {
		if (th.joinable()) {
			th.join();
		}
	}

	this->aFirstUpdate = 1;

}


/*
		4. Destructor
Purpose: Deletes all macroNodes called by new keyword
Inputs: None
Return Type: None
*/
OccupancyGrid::~OccupancyGrid() {
	// Create a new pointer for GridNode
	MacroNode* Temp;
	for (int node = 0; node < Tailor.aMacroNodeList.size(); node++) {
		// get the macroNode address and assign it to NULL
		Temp = Tailor.aMacroNodeList[node];
		Tailor.aMacroNodeList[node] = NULL;
		delete(Temp);
	}
}



/////////////// Private  ///////////////////////
/*
		1.  VerticalScan
Purpose: Finds required point while expanding grid
Inputs: _x - x coordinate (Reference)
		_Location - MacroNode* Pointer
Return Type: None
*/
void OccupancyGrid::VerticalScan(const long long& _x, MacroNode*& _Location) {
	if (_x >= _Location->aTopLeft.first) {
		// this implies move north if top node exists
		if (_Location->pNorth != NULL) {
			_Location = _Location->pNorth;
		}
		else {
			// Expand the Node
			Tailor.ExpandMacroNode(_Location);
			_Location = _Location->pNorth;
		}
	}
	else {
		// This implies move south
		if (_Location->pSouth != NULL) {
			_Location = _Location->pSouth;
		}
		else {
			// Expand the Node
			Tailor.ExpandMacroNode(_Location);
			_Location = _Location->pSouth;
		}
	}
}


/*
		2.  Horizontal Scan
Purpose: Finds required point while expanding grid
Inputs: _y - y coordinate (Reference)
		_Location - MacroNode* Pointer
Return Type: None
*/
void OccupancyGrid::HorizontalScan(const long long& _y, MacroNode*& _Location) {
	if (_y > _Location->aTopLeft.second) {
		// this implies move west if left node exists
		if (_Location->pWest != NULL) {
			_Location = _Location->pWest;
		}
		else {
			// Expand the Node
			Tailor.ExpandMacroNode(_Location);
			_Location = _Location->pWest;
		}
	}
	else {
		// This implies move East
		if (_Location->pEast != NULL) {
			_Location = _Location->pEast;
		}
		else {
			// Expand the Node
			Tailor.ExpandMacroNode(_Location);
			_Location = _Location->pEast;
		}
	}
}

/*
		3. UpdatePolarGrid
Purpose: Updates grid based on Lidar measurements
Inputs: _measurements - Lidar reading
Return Type: None
*/
void OccupancyGrid::UpdatePolarGrid(vector<floatPair> _measurements) {
	// call the polargrid Update function
	vector<long long> State = this->pStateEstimator->GetState();
	this->aGridPolar.Update(_measurements, State);
}

/*
		4. UpdatePosition
Purpose: Updates Point variables in the class for location and bottomleft
Inputs: None
Return Type: None
*/
void OccupancyGrid::UpdatePosition() {
	// Get the state
	vector<long long> State = pStateEstimator->GetState();
	aVehicleLocation = FindPoint(State[0], State[1]);
	// Now find the bottom left node
	aBottomLeftLocation = FindPoint(State[0] - 3, State[1] + 3);

}


/*
		5. FindMacroIndex
Purpose: Given a cell in Cartesian Grid, find the Point object
Inputs: _xIndex - Row index
		_yIndex - Column index
Return Type: Point object
*/
Point OccupancyGrid::FindMacroIndex(int _xIndex, int _yIndex) {
	// create empty point
	Point result;
	result.pLocation = aBottomLeftLocation.pLocation;

	// The row and column indices are the modulus operation with
	// size of MacroNode
	result.aXIndex = (_xIndex + aBottomLeftLocation.aXIndex) %
		(result.pLocation->aSizeX);
	result.aYIndex = (_yIndex + aBottomLeftLocation.aYIndex) %
		(result.pLocation->aSizeY);

	// Calculate number of shifts in horizontal and vertical
	int numShiftsX = (_xIndex + aBottomLeftLocation.aXIndex) /
		(result.pLocation->aSizeX);
	int numShiftsY = (_yIndex + aBottomLeftLocation.aYIndex) /
		(result.pLocation->aSizeY);

	// Shift Vertically
	for (int shift = 0; shift < numShiftsX; shift++) {
		if (result.pLocation->pNorth != NULL) {
			result.pLocation = result.pLocation->pNorth;
		}
		else {
			Tailor.ExpandMacroNode(result.pLocation);
			result.pLocation = result.pLocation->pNorth;
		}
	}

	// Shift Horizontally
	for (int shift = 0; shift < numShiftsY; shift++) {
		if (result.pLocation->pEast != NULL) {
			result.pLocation = result.pLocation->pEast;
		}
		else {
			Tailor.ExpandMacroNode(result.pLocation);
			result.pLocation = result.pLocation->pEast;
		}
	}


	// Return the point
	return result;
}

/*
		6. InverseSensorModel
Purpose: Given a cell in Cartesian Grid, returns the required
			log odds update
Inputs: _xIndex - Row index
		_yIndex - Column index in CartesianGrid
Return Type: float - log odds value
*/
float OccupancyGrid::InverseSensorModel(const int& _xIndex, const int& _yIndex) {
	// create float variable
	float logodds = 0.0;
	// Get the cartesianNode pointer
	CartesianNode* current = &aGridCart.aGrid[_xIndex][_yIndex];
	// Get the occupancy from the polarNode pointer
	int occupancy = current->GetPolarPointer()->GetOccupancy();
	// Define log odds for occupancy values
	if (occupancy == 1) {
		// unknown
		logodds = std::log(1.0);
		logodds = std::round(logodds);
		return logodds;
	}
	if (occupancy == 0) {
		// free cell
		logodds = std::log(0.05 / 0.95);
		return logodds;
	}
	if (occupancy == 2) {
		// occupiedcell 
		logodds = std::log(0.9 / 0.1);

	}
	return logodds;
}

/*
		7. UpdateLogOdds
Purpose: Given a Point object and log odds, update corresponding cell
Inputs: _Node - Point object referencing cell to be updated.
		_LogOdds - LogOdds value to be added.
Return Type: None
*/
void OccupancyGrid::UpdateLogOdds(const Point& _Node, const float& _LogOdds) {
	// Update the required cell
	_Node.pLocation->aGrid[_Node.aXIndex][_Node.aYIndex].
		UpdateLogOdds(_LogOdds);
}

/*
		8. UpdateRowOccupancy
Purpose: Updates three rows of the grid
Inputs: _StartIndex - Row index of first Row
Return Type: None
*/
void OccupancyGrid::UpdateRowOccupancy(int _StartIndex) {
	Point UpdateNode;
	float LogOdds;
	// Loop through the three rows
	for (int row = _StartIndex; row < _StartIndex + 3; row++) {
		// Loop through the columns
		for (int col = 0; col < aGridCart.aGrid[0].size(); col++) {
			// For every element, find the point object
			UpdateNode = FindMacroIndex(row, col);
			// First check if in blindspot
			if (aGridCart.aGrid[row][col].GetPolarPointer() == NULL) {
				if (aFirstUpdate == 0) {
					// If in blindspot, at first reading, within 150 mm is 
					// assumed free
					if (aGridCart.aGrid[row][col].aOriginDistance < 151) {
						LogOdds = std::log(0.05 / 0.95);
						UpdateLogOdds(UpdateNode, LogOdds);
					}
				}
			}
			else {
				// This means it is in sensor range
				LogOdds = InverseSensorModel(row, col);
				UpdateLogOdds(UpdateNode, LogOdds);
			}


		}
	}
}

/*
		9. CreateInitialGrid()
Purpose: Creates the initial 20x20m grid
Inputs: None
Return Type: None
*/
void OccupancyGrid::CreateInitialGrid() {
	long long startX = -20000;
	long long startY = -20000;
	long long xCoordinate = 0;
	long long yCoordinate = 0;
	MacroNode* Current = NULL;
	// Loop through X coordinates
	for (int multiplierX = 0; multiplierX <= 20; multiplierX++) {
		// Loop through Y coordinates
		for (int multiplierY = 0; multiplierY <= 20; multiplierY++) {
			xCoordinate = startX + (multiplierX * (long long)2000);// every time grid expands by 2m
			yCoordinate = startY + (multiplierY * (long long)2000);
			if (!((xCoordinate == 0) && (yCoordinate == 0))) {
				// if not origin, create new macroNode and add
				Current = new MacroNode(xCoordinate, yCoordinate);
				Tailor.AddMacroNode(Current);
			}
		}
	}

}