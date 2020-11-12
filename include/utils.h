/*
	This file will contain all the helper class declarations.
	The classes that will be defined are:
	1. Localization class which holds vehicle pose.
	2. Point class which holds the point object
*/

#ifndef UTILS_H
#define UTILS_H

#include"GridNode.h"
#include"Eigen/Dense"
#include<vector>

using Eigen::Vector3f;

/*
	Class Name: Point
	Function: Hold a MacroNode pointer and indices for x and y
*/
class Point {
public:
	Point() :pLocation(NULL), aXIndex(0), aYIndex(0) {

	}
	MacroNode* pLocation;
	int aXIndex;
	int aYIndex;
};

/*
	Class Name: Localization
	Function: Contains and updates state of the wheeled robot
*/
class Localization {
public:
	Localization() :aX(0), aY(0), aTheta(0) {

	}

	std::vector<long long> GetState() const {
		return std::vector<long long>({ aX,aY,aTheta });
	}

	void AssignState(const Vector3f& _state) {
		aX = (long long)_state(0);
		aY = (long long)_state(1);
		aTheta = (long long)_state(2);
	}


protected:
	long long aX;
	long long aY;
	long long aTheta;
};

#endif // !UTILS_H
