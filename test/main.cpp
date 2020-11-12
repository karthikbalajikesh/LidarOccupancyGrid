#include<iostream>
#include"Node.h"
#include"GridNode.h"
#include"Stitcher.h"
#include"PolarNode.h"
#include"PolarGrid.h"
#include"OccupancyGrid.h"
#include"CartesianGrid.h"
#include"utils.h"
#include<unordered_map>
#include<chrono>


using namespace std::chrono;

using std::cout;
using std::endl;
using std::vector;

void TestStitcher() {
	Stitcher A;
	MacroNode* temp = new MacroNode();
	A.AddMacroNode(temp);
	A.ExpandMacroNode(temp);
}

void TestPolarNode() {
	PolarNode A;
	cout << "Occupancy of Polar Node is  " << A.GetOccupancy() << endl;
	cout << "The bounds of the Node are as follows:  " << endl;
	vector<int> bounds = A.GetBounds();
	cout << " Max Range =  " << bounds[0] << endl;
	cout << " Min Range =  " << bounds[1] << endl;
	cout << " Max Angle =  " << bounds[2] << endl;
	cout << " Min Angle =  " << bounds[3] << endl;
}

void TestCartesianGrid() {
	PolarGridRP A;
	CartesianGrid B(&A);

	return;

}

void TestPolarGrid() { 
	PolarGridRP A;
	vector<floatPair> Measurements;
	Measurements.push_back({ 169.25,3.58 });
	vector<long long> state({ 0,0,0 });
	A.Update(Measurements, state);
	return;
}

void TestOccupancyGrid() {
	Localization SE;
	OccupancyGrid A(&SE);
	// simulate moving
	Eigen::Vector3f State;
	State << 1000, 3000, 142;
	// simulating no lidar measurements
	std::vector<std::pair<float, float>> measurements(0);
	measurements.push_back({ 452.36,52.15 });
	measurements.push_back({ 187.22,12.88 });
	measurements.push_back({ 324,154 });
	SE.AssignState(State);


	A.Update(measurements);
	
	std::cout << "Done\n";

	return;
}


//////////////////////////////////////////////////////////////
int main()
{
	TestOccupancyGrid();

	return 0;
}