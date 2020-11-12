/*
This file contains declarations for the Stitcher class.

Purpose: Maintain a connected structure of the Macros structure.
*/

#ifndef STITCHER_H
#define STITCHER_H

#include"Node.h"
#include"GridNode.h"
#include<unordered_map>
#include<map>

using std::map;
using std::pair;
using std::unordered_map;
using std::vector;


class Stitcher {
public:
	//methods
	// Default Constructor
	Stitcher() :aMacroHeight{ 2000 }, aMacroWidth{ 2000 }{

	}

	/*
						AddMacroNode()
		Purpose: Adds new node to the hashmap and the vector and updates neighbors
		Input: NewMacro - pointer of the newly created MacroNode
	*/
	void AddMacroNode(MacroNode* NewMacro) {
		// Add new node to the Hashmap and vector
		longPair center = NewMacro->GetCenterCoordinates();
		long long xCoordinate = center.first;
		long long yCoordinate = center.second;
		this->aMacroHash[xCoordinate][yCoordinate] = NewMacro;
		aMacroNodeList.push_back(NewMacro);

		// check four neighbors

		checkNorthNeighbor(NewMacro, center);
		checkSouthNeighbor(NewMacro, center);
		checkEastNeighbor(NewMacro, center);
		checkWestNeighbor(NewMacro, center);
	}


	/*
						ExpandMacroNode()
		Purpose: Expands a current MacroNode if needed
		Input: CurrentMacro - pointer of MacroNode to be expanded
	*/
	void ExpandMacroNode(MacroNode* CurrentMacro) {
		// Check North
		if (CurrentMacro->pNorth == NULL) {
			longPair center = CurrentMacro->GetCenterCoordinates();
			center.first += this->aMacroHeight;
			MacroNode* NewMacro = new MacroNode(center.first, center.second);
			AddMacroNode(NewMacro);
		}

		// Check East
		if (CurrentMacro->pEast == NULL) {
			longPair center = CurrentMacro->GetCenterCoordinates();
			center.second -= this->aMacroWidth;
			MacroNode* NewMacro = new MacroNode(center.first, center.second);
			AddMacroNode(NewMacro);
		}

		// Check South
		if (CurrentMacro->pSouth == NULL) {
			longPair center = CurrentMacro->GetCenterCoordinates();
			center.first -= this->aMacroHeight;
			MacroNode* NewMacro = new MacroNode(center.first, center.second);
			AddMacroNode(NewMacro);
		}

		// Check West
		if (CurrentMacro->pWest == NULL) {
			longPair center = CurrentMacro->GetCenterCoordinates();
			center.second += this->aMacroWidth;
			MacroNode* NewMacro = new MacroNode(center.first, center.second);
			AddMacroNode(NewMacro);
		}
	}


	// attributes
	// 1. 2D hash with X and Y in order
	unordered_map<long long, unordered_map<long long, MacroNode*>> aMacroHash;

	// 2. List of MacroNodes
	vector<MacroNode*> aMacroNodeList;

protected:
	int aMacroHeight;
	int aMacroWidth;

	/*
						Check North neighbor
		Purpose: Function checks if any node to the north is in map and stitches them
					together.
	*/
	void checkNorthNeighbor(MacroNode* NewMacro, longPair center) {

		// Calculate center for north 
		long long Xcoordinate = center.first + aMacroHeight;
		long long Ycoordinate = center.second;

		if (aMacroHash.find(Xcoordinate) != aMacroHash.end()) {
			// passed the x coordinate
			if (aMacroHash[Xcoordinate].find(Ycoordinate) != aMacroHash[Xcoordinate].end()) {
				MacroNode* Neighbor = aMacroHash[Xcoordinate][Ycoordinate];
				Neighbor->pSouth = NewMacro;
				NewMacro->pNorth = Neighbor;
			}

		}
	}

	/*
						Check South neighbor
		Purpose: Function checks if any node to the south is in map and stitches them
					together.
	*/
	void checkSouthNeighbor(MacroNode* NewMacro, longPair center) {

		// Calculate center for south 
		long long Xcoordinate = center.first - aMacroHeight;
		long long Ycoordinate = center.second;

		if (aMacroHash.find(Xcoordinate) != aMacroHash.end()) {
			// passed the x coordinate
			if (aMacroHash[Xcoordinate].find(Ycoordinate) != aMacroHash[Xcoordinate].end()) {
				MacroNode* Neighbor = aMacroHash[Xcoordinate][Ycoordinate];
				Neighbor->pNorth = NewMacro;
				NewMacro->pSouth = Neighbor;
			}

		}
	}

	/*
						Check East neighbor
		Purpose: Function checks if any node to the East is in map and stitches them
					together.
	*/
	void checkEastNeighbor(MacroNode* NewMacro, longPair center) {

		// Calculate center for east 
		long long Xcoordinate = center.first;
		long long Ycoordinate = center.second - this->aMacroWidth;

		if (aMacroHash.find(Xcoordinate) != aMacroHash.end()) {
			// passed the x coordinate
			if (aMacroHash[Xcoordinate].find(Ycoordinate) != aMacroHash[Xcoordinate].end()) {
				MacroNode* Neighbor = aMacroHash[Xcoordinate][Ycoordinate];
				Neighbor->pWest = NewMacro;
				NewMacro->pEast = Neighbor;
			}

		}
	}


	/*
						Check West neighbor
		Purpose: Function checks if any node to the west is in map and stitches them
					together.
	*/
	void checkWestNeighbor(MacroNode* NewMacro, longPair center) {

		// Calculate center for west 
		long long Xcoordinate = center.first;
		long long Ycoordinate = center.second + this->aMacroWidth;

		if (aMacroHash.find(Xcoordinate) != aMacroHash.end()) {
			// passed the x coordinate
			if (aMacroHash[Xcoordinate].find(Ycoordinate) != aMacroHash[Xcoordinate].end()) {
				MacroNode* Neighbor = aMacroHash[Xcoordinate][Ycoordinate];
				Neighbor->pEast = NewMacro;
				NewMacro->pWest = Neighbor;
			}

		}
	}
};
#endif // !STITCHER_H
