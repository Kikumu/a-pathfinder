#pragma once

#include <fstream>
#include <string>
#include "Node.h"

class Navigation
{
	std::ofstream _outFile;

	// Add your code here

public:
	Navigation();
	~Navigation();

    bool BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks)const ;
	bool ProcessCommand(const std::string& commandString);



	//list of unvisited nodes/arcs. if this list runs empty then the solution has either been found or no soln available
	//void openset(vector<Node*>);

	//list of visited nodes/arcs
	//void closedset(vector<Node*>);
	// Add your code here
};