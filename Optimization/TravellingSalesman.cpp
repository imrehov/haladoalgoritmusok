#include "stdafx.h"
#include "TravellingSalesman.h"
#include <fstream>
#include <string>

//
// Basic IO
//
void TravellingSalesmanProblem::loadTownsFromFile(std::string fileName) {
	std::ifstream ftowns(fileName);
	while (!ftowns.eof()) {
		Town town;
		ftowns >> town.x;
		ftowns >> town.y;
		towns.push_back(town);
	};
	ftowns.close();
}

void TravellingSalesmanProblem::saveTownsToFile(std::string fileName, std::vector<Town> townVector) {
	std::ofstream ftowns(fileName);
	for (Town twn : townVector) {
		ftowns << twn.x << "\t" << twn.y << std::endl;
	};
	ftowns.close();
}

//
// Objective functions
//
float TravellingSalesmanProblem::objective(std::vector<Town> route) {
	float sum_length = 0;

	for (unsigned int ti = 0; ti < route.size() - 1; ti++) {
		Town t1 = route[ti];
		Town t2 = route[ti + 1];
		sum_length += sqrt(pow(t1.x - t2.x, 2) + pow(t1.y - t2.y, 2));
	}
	return sum_length;
}


