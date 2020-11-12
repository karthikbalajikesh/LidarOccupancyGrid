/*
The header files contain the declaration of the abstract class Node and
the 2x2 cm cell class Node2x2.

Node is the fine cell that is present in every 2x2 m grid cell.

Purpose:
	Hold and publish LogOdds value
	Hold and publish coordinate value
*/

#ifndef NODE_H
#define NODE_H

#include<utility>
#include<vector>

//Macros
#define longPair std::pair<long long,long long> 

using std::vector;
using std::pair;

class Node {
public:
	// methods	
	// 3. Function that updates log odds
	virtual void UpdateLogOdds(const float& _logOdds) = 0;

	// 4. Function that returns log odds
	virtual float GetLogOdds() const = 0;

	// 5. Function that returns coordinates of centroid
	virtual longPair GetCoordinates() const = 0;

	// 6. Function that checks whether a coordinate is present in cell
	virtual bool CheckPresence(const long long& _x, const long long& _y) = 0;

	// 7. Function that returns the bounds of x and y coordinates
	virtual vector<long long> GetBounds() const = 0;


};

/*
Class Node2x2 contains the 2x2 cell implementation of the interface Node.

Details:
	ResolutionX = ResolutionY = 20mm = 2cm
*/

class Node2x2 :public Node {
public:
	// methods
	// 1.  Default Constructor
	Node2x2();

	// 2. Constructor which accepts coordinates
	Node2x2(const long long& _xCoordinate, const long long& _yCoordinate);

	// 3. Function that updates log odds
	void UpdateLogOdds(const float& _logOdds) override;

	// 4. Function that returns log odds
	float GetLogOdds() const override;

	// 5. Function that returns coordinates of centroid
	longPair GetCoordinates() const override;

	// 6. Function that checks if coordinate is present in cell
	bool CheckPresence(const long long& _x, const long long& _y) override;

	// 7. Function that returns the bounds of x and y coordinates
	vector<long long> GetBounds() const override;

protected:
	// Attributes

	// 1. Coordinates in mm in (x,y) format
	longPair aCoordinate;

	// 2. Resolution in x axis in mm
	int aResolutionX;

	// 3. Resolution in y axis in mm
	int aResolutionY;

	// 4. Log odds of occupation
	float aLogOdds;

};

#endif