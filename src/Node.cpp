/*
File contains implementations of the Node2x2 class
*/

#include"Node.h"

// 1. Default Constructor
Node2x2::Node2x2() : aCoordinate({ 0,0 }),
aResolutionX(20), aResolutionY(20),
aLogOdds(0.0) {}


// 2. Constructor with Coordinate as parameter
Node2x2::Node2x2(const long long& _xCoordinate, const long long& _yCoordinate) :
	aCoordinate({ _xCoordinate,_yCoordinate }),
	aResolutionX(20), aResolutionY(20),
	aLogOdds(0.0) {}


// 3. Function that updates LogOdds
void Node2x2::UpdateLogOdds(const float& _logOdds) {
	/*
	Function adds the log Odds value to current log odds
	*/
	this->aLogOdds += _logOdds;
}


// 4. Function that returns log Odds for path planning
float Node2x2::GetLogOdds() const {
	/*
	Function returns the log Odds value held by class
	*/
	return aLogOdds;
}


// 5. Function that returns the coordinate of centroid
longPair Node2x2::GetCoordinates() const {
	/*
	Function returns the pair of coordinates of center
	in {x,y} form
	*/
	return aCoordinate;
}


// 6. Function that checks for presence of coordinate in cell
//    for Binary search
bool Node2x2::CheckPresence(const long long& _x,
	const long long& _y) {
	/*
	Function returns true if coordinate is present in cell
	*/
	if (_x < (aCoordinate.first + (aResolutionX / 2)) &&
		_x >= (aCoordinate.first - (aResolutionX / 2))) {
		// Enters when _x belongs to [xmin,xmax)

		if (_y > (aCoordinate.second - (aResolutionY / 2)) &&
			(_y <= (aCoordinate.second + (aResolutionY / 2)))) {
			// Enters when _y belongs to (ymin,ymax]

			return true;
		}
	}
	return false;
}

// 7. Function that returns the bounds of x and y coordinates
vector<long long> Node2x2::GetBounds() const {
	/*
	Function returns bounds in the order
	xmax
	xmin
	ymax
	ymin
	*/

	std::vector<long long> Bounds(4, 0);
	Bounds[0] = aCoordinate.first + (aResolutionX / 2);
	Bounds[1] = aCoordinate.first - (aResolutionX / 2);
	Bounds[2] = aCoordinate.second + (aResolutionY / 2);
	Bounds[3] = aCoordinate.second - (aResolutionY / 2);

	return Bounds;
}

