#include "Navigation.h"
#include "Node.h"
#include "Arc.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include<algorithm>

using namespace std;


Node ACCESS;
vector<Node*>MyNodes; //node class data
vector<Arc*>MyArcs; //links

//vector<Arc*>destinys;
vector<Arc*>arc_temp;
//vector<Node*>places_temp1;
vector<double>distances;

vector<string>tokens;
vector<string>splitter_1;

vector<Node*>openset;
vector<Node*>closedset;
vector<Node*>NeighbourRef;

vector<Arc*>opensetArc;
vector<double>temporaryDistances;
Node* Xtemp = new Node;
Node* Ytemp = new Node;
Arc*dist = new Arc;

Node* backtracked_pointer = new Node;
bool correctArc;
bool foundNonDuplicate;
bool opensetchecker;
bool tempArcSelector;
bool tempPointerTest;
bool tempPointerTest1;
// Converts latitude/longitude into eastings/northings
int counter;
//int itera = 0, iterb = 0, iterc = 0;
extern void LLtoUTM(const double Lat, const double Long, double &UTMNorthing, double &UTMEasting);

//if value in product false if value not product true
bool closedsetchecker(vector<Node*> & p, const Node*  const nptr)
{
	for (int i = 0; i != p.size(); i++)
	{
		if (nptr->getRef() == p[i]->getRef())
		{
			return false;
			break;
		}	
	}
	return true;
}

bool Checker(const Arc* const a, vector<Node*> & n)
{
	Xtemp = NULL;
	Ytemp = NULL;
	for (int i = 0; i != n.size(); i++)
	{
		if (a->get_node() == n[i])
			Xtemp = n[i];
		if (a->get_node1() == n[i])
			Ytemp = n[i];
	}

	if ((Xtemp!=NULL) && (Ytemp!= NULL)) //returns false if both values are in my closed set
		return false;
	else
		return true;
}

//erases element from openset
vector<Node*> openset_eraser(vector<Node*>&p, const Node * const pntr2)
{
	int element = 0;
	for (int x = 0; x != p.size(); x++)
	{
		if (p[x] == pntr2)
		{
			element = x;
		}
	}
	p.erase(p.begin() + element);
	return p;
}

//takes previous element
Node* backtracker(vector<Node*>&p, const Node * const pntr2)
{
	int element = 0;
	int element_before = 0;
	//finds pointer 2 and returns the pointer before it
	//finds element
	for (int x = 0; x != p.size(); x++)
	{
		if (p[x] == pntr2)
		{
			element = x;
			element_before = (element - 1);
		}
	}

	for (int f = 0; f != p.size(); f++)
	{
		if (f == element_before)
			backtracked_pointer = p[f];
	}
	return backtracked_pointer;
}


double distance(double const x,double const x1, double const y, double const y1)
{
	return sqrt(((x1 - x)*(x1 - x)) + ((y1 - y) * (y1 - y)));
}

vector<string> splitter(string &w, char const delimiter)  //splitter function
{
	string token;
	std::stringstream tokenstream(w);
	while (getline(tokenstream, token, delimiter))
		tokens.push_back(token);
	return tokens;                                   //returns splitted string inside a vector 
}

Navigation::Navigation() : _outFile("Output.txt")
{
}

Navigation::~Navigation()
{
}

int refs, refe, refp,size1;
double get_x1, get_x2, get_y1, get_y2,minval;
string maxplc1, maxplc2,transport;
bool Navigation::ProcessCommand(const string& commandString)
{
	istringstream inString(commandString);
	string command;
	inString >> command;
	int startNodeID, endNodeID,refID_1,refID_2;
	//string transport;

	if (command == "MaxDist")
	{
		for (int j = 0; j != MyArcs.size(); j++)
		{
			refs = MyArcs[j]->getStart();
			refe = MyArcs[j]->getEnd();
			for (int k = 0; k != MyNodes.size(); k++)
			{
				if (refs == MyNodes[k]->getRef())
				{
					get_x1 = MyNodes[k]->get_xval();
					get_y1 = MyNodes[k]->get_yval();
				}
				else if (refe == MyNodes[k]->getRef())
				{
					get_x2 = MyNodes[k]->get_xval();
					get_y2 = MyNodes[k]->get_yval();
				}
			}
			double const  tempDist = (distance(get_y1, get_y2, get_x1, get_x2));
			distances.push_back(tempDist);
		    MyArcs[j]->setdistance(tempDist);
		}

		double maxval = distances[0];


		//finds maximum value
		for (int m = 1; m != distances.size(); m++)
		{
			if (distances[m] > maxval)
				maxval = distances[m];
		}
	

		for (int z = 0; z != MyArcs.size(); z++)
		{
			if (maxval <= MyArcs[z]->get_distance())
			{
				dist = MyArcs[z];
				for (int x = 0; x != MyNodes.size(); x++)
				{
					if (MyArcs[z]->getStart() == MyNodes[x]->getRef())
						maxplc1 = MyNodes[x]->getPlace();
					else if (MyArcs[z]->getEnd() == MyNodes[x]->getRef())
						maxplc2 = MyNodes[x]->getPlace();
				}
				break;
		    }
		}
		distances.clear();
		_outFile << maxplc1 << ","<< maxplc2 << ","<< fixed << showpoint << setprecision(3) << maxval << "\n"<<endl;
		return true;
	}

	else if (command == "FindDist")
	{
		inString >> refID_1;
		inString >> refID_2;

		for (int x = 0; x != MyArcs.size(); x++)
		{
			if ((MyArcs[x]->getEnd() == refID_1 && MyArcs[x]->getStart() == refID_2) || (MyArcs[x]->getEnd() == refID_2 && MyArcs[x]->getStart() == refID_1))
			{
				get_y1 = MyArcs[x]->get_distance();
				_outFile << get_y1 <<"\n"<< endl;
				return true;
			}
		}
	}

	
	else if (command == "Check")
	{

		inString >> transport;
		inString >> startNodeID;
		inString >> endNodeID;
		inString >> refID_1;
		inString >> refID_2;
		_outFile << command << " " <<transport<<" "<< startNodeID << " " << endNodeID << " " << refID_1 << " " << refID_2 << endl;

		if ((transport == "Rail")||(transport== "Ship"))
		{
			for (int a = 0; a != MyArcs.size(); a++)
			{
				//intera++;
				if (transport == MyArcs[a]->getMode() && ((startNodeID == MyArcs[a]->getStart() && endNodeID == MyArcs[a]->getEnd()) || (startNodeID == MyArcs[a]->getEnd() && endNodeID == MyArcs[a]->getStart())))
				{
					_outFile << startNodeID << "," << endNodeID << "," << "PASS" << endl;
					//end node is startmode and refid1 is endmode
					for (int b = 0; b != MyArcs.size(); b++)
					{
						//interb++;
						if (transport == MyArcs[b]->getMode() && ((endNodeID == MyArcs[b]->getStart() && refID_1 == MyArcs[b]->getEnd()) || (endNodeID == MyArcs[b]->getEnd() && refID_1 == MyArcs[a]->getStart())))
						{
							_outFile << endNodeID << "," << refID_1 << "," << "PASS" << endl;
							//refid1 is endnode and refid2 is refid1
							for (int c = 0; c != MyArcs.size(); c++)
							{
								//++;
								if (transport == MyArcs[c]->getMode() && ((refID_1 == MyArcs[c]->getStart() && refID_2 == MyArcs[c]->getEnd()) || (refID_1 == MyArcs[c]->getEnd() && refID_2 == MyArcs[c]->getStart())))
								{
									_outFile << refID_1 << "," << refID_2 << "," << "PASS\n" << endl;
									break;
								}
							}
							break;
						}
					}
					break;
				}
				else if (a == 350)
				{
					_outFile << startNodeID << "," << endNodeID << "," << "FAIL\n" << endl;
					break;
				}
			}
			return true;
		}
		else if (transport == "Bus") //can only use bus and ship arc
		{
			for (int a = 0; a != MyArcs.size(); a++)
			{
				if ((transport == MyArcs[a]->getMode()|| transport =="Ship") && ((startNodeID == MyArcs[a]->getStart() && endNodeID == MyArcs[a]->getEnd()) || (startNodeID == MyArcs[a]->getEnd() && endNodeID == MyArcs[a]->getStart())))
				{
					_outFile << startNodeID << "," << endNodeID << "," << "PASS" << endl;
					//end node is startmode and refid1 is endmode
					for (int b = 0; b != MyArcs.size(); b++)
					{
						if ((transport == MyArcs[b]->getMode()|| transport == "Ship") && ((endNodeID == MyArcs[b]->getStart() && refID_1 == MyArcs[b]->getEnd()) || (endNodeID == MyArcs[b]->getEnd() && refID_1 == MyArcs[a]->getStart())))
						{
							_outFile << endNodeID << "," << refID_1 << "," << "PASS" << endl;
							//refid1 is endnode and refid2 is refid1
							for (int c = 0; c != MyArcs.size(); c++)
							{
								if ((transport == MyArcs[c]->getMode()|| transport=="Ship") && ((refID_1 == MyArcs[c]->getStart() && refID_2 == MyArcs[c]->getEnd()) || (refID_1 == MyArcs[c]->getEnd() && refID_2 == MyArcs[c]->getStart())))
								{
									_outFile << refID_1 << "," << refID_2 << "," << "PASS\n" << endl;
									break;
								}
							}
							break;
						}
					}
					break;
				}
				else if (a == 350)
				{
					_outFile << startNodeID << "," << endNodeID << "," << "FAIL\n" << endl;
					break;
				}
			}
			return true;
		}
		else if (transport == "Foot") //any arc
		{
			for (int a = 0; a != MyArcs.size(); a++)
			{
				if (((startNodeID == MyArcs[a]->getStart() && endNodeID == MyArcs[a]->getEnd()) || (startNodeID == MyArcs[a]->getEnd() && endNodeID == MyArcs[a]->getStart())))
				{
					_outFile << startNodeID << "," << endNodeID << "," << "PASS" << endl;
					//end node is startmode and refid1 is endmode
					for (int b = 0; b != MyArcs.size(); b++)
					{
						if (((endNodeID == MyArcs[b]->getStart() && refID_1 == MyArcs[b]->getEnd()) || (endNodeID == MyArcs[b]->getEnd() && refID_1 == MyArcs[a]->getStart())))
						{
							_outFile << endNodeID << "," << refID_1 << "," << "PASS" << endl;
							//refid1 is endnode and refid2 is refid1
							for (int c = 0; c != MyArcs.size(); c++)
							{
								if (((refID_1 == MyArcs[c]->getStart() && refID_2 == MyArcs[c]->getEnd()) || (refID_1 == MyArcs[c]->getEnd() && refID_2 == MyArcs[c]->getStart())))
								{
									_outFile << refID_1 << "," << refID_2 << "," << "PASS\n" << endl;
								break;
								}
							}
						break;
						}
					}
				break;
				}
				else if (a == 350)
				{
					_outFile << startNodeID << "," << endNodeID << "," << "FAIL\n" << endl;
					break;
				}
			}
			return true;
		}
		else if (transport == "Bike")//all arcs except foot
		{
		for (int a = 0; a != MyArcs.size(); a++)
		{
			if ((transport!="Foot") && ((startNodeID == MyArcs[a]->getStart() && endNodeID == MyArcs[a]->getEnd()) || (startNodeID == MyArcs[a]->getEnd() && endNodeID == MyArcs[a]->getStart())))
			{
				_outFile << startNodeID << "," << endNodeID << "," << "PASS" << endl;
				//end node is startmode and refid1 is endmode
				for (int b = 0; b != MyArcs.size(); b++)
				{
					if ((transport != "Foot") && ((endNodeID == MyArcs[b]->getStart() && refID_1 == MyArcs[b]->getEnd()) || (endNodeID == MyArcs[b]->getEnd() && refID_1 == MyArcs[a]->getStart())))
					{
						_outFile << endNodeID << "," << refID_1 << "," << "PASS" << endl;
						//refid1 is endnode and refid2 is refid1
						for (int c = 0; c != MyArcs.size(); c++)
						{
							if ((transport != "Foot") && ((refID_1 == MyArcs[c]->getStart() && refID_2 == MyArcs[c]->getEnd()) || (refID_1 == MyArcs[c]->getEnd() && refID_2 == MyArcs[c]->getStart())))
							{
								_outFile << refID_1 << "," << refID_2 << "," << "PASS\n" << endl;
								break;
							}
						}
						break;
					}
				}
				break;
			}
			else if (a == 350)
			{
				_outFile << startNodeID << "," << endNodeID << "," << "FAIL\n" << endl;
				break;
			}
		}
		return true;
        }
		else if (transport == "Car") //car,bus and ship, cannot use bike/foot/rail
		{
		for (int a = 0; a != MyArcs.size(); a++)
		{
			if (((transport == "Car") || (transport=="Bus")||(transport=="Ship")) && ((startNodeID == MyArcs[a]->getStart() && endNodeID == MyArcs[a]->getEnd()) || (startNodeID == MyArcs[a]->getEnd() && endNodeID == MyArcs[a]->getStart())))
			{
				_outFile << startNodeID << "," << endNodeID << "," << "PASS" << endl;
				//end node is startmode and refid1 is endmode
				for (int b = 0; b != MyArcs.size(); b++)
				{
					if (((transport != "Foot") || (transport != "Bike") || (transport != "Rail"))&& ((endNodeID == MyArcs[b]->getStart() && refID_1 == MyArcs[b]->getEnd()) || (endNodeID == MyArcs[b]->getEnd() && refID_1 == MyArcs[a]->getStart())))
					{
						_outFile << endNodeID << "," << refID_1 << "," << "PASS" << endl;
						//refid1 is endnode and refid2 is refid1
						for (int c = 0; c != MyArcs.size(); c++)
						{
							if (((transport != "Foot") || (transport != "Bike") || (transport != "Rail")) && ((refID_1 == MyArcs[c]->getStart() && refID_2 == MyArcs[c]->getEnd()) || (refID_1 == MyArcs[c]->getEnd() && refID_2 == MyArcs[c]->getStart())))
							{
								_outFile << refID_1 << "," << refID_2 << "," << "PASS\n" << endl;
								break;
							}
						}
						break;
					}
				}
				break;
			}
			else if (a == 350)
			{
				_outFile << startNodeID << "," << endNodeID << "," << "FAIL\n" << endl;
				break;
			}
		}
		return true;
        }
		
		
	}

	else if (command == "FindNeighbour")
	{
		inString >> refID_1;
		_outFile << command << refID_1 << endl;
		for (int i = 0; i != MyArcs.size(); i++)
		{
			if (MyArcs[i]->getStart() == refID_1)
			{
				refID_2 = MyArcs[i]->getEnd();
				_outFile << refID_2 << endl;
			}
			else if (MyArcs[i]->getEnd() == refID_1)
			{
				refID_2 = MyArcs[i]->getStart();
				_outFile << refID_2 << endl;
			}
		}
		_outFile << "\n" << endl;
		return true;
	}

	else if ((command == "FindRoute")||(command == "FindShortestRoute"))
	{
	//i want it to keep going to the next node until it finds desired node
	inString >> transport;
	inString >> startNodeID;
	inString >> endNodeID;

	_outFile << command << " " << transport <<" "<< startNodeID << " " << endNodeID << endl;
	Node* start = new Node;
	Node* end = new Node;

	//obtain destination values and start values from the nodes and store them in a pointer
	for (int i = 0; i != MyNodes.size(); i++)
	{
		if (MyNodes[i]->getRef() == startNodeID)
		{
			start = MyNodes[i];
			refs = i;
		}
		else if (MyNodes[i]->getRef() == endNodeID)
		{
			end = MyNodes[i];
		}
	}

	//if you fuck things up turn to latest whatever 1 in your desktop
	openset.push_back(start);
	Node* previous = new Node;
	Node*current = new Node;
	Arc*temporaryPointer = new Arc;
	
	while (openset.size() != 0)
	{
		
		    //keep searching. if nothing is no longer in our open set this loop breaks
		    //wanna evaluate all openset possibilities(neighbours) and look for shortest distance g, put in winner and go to it 
		    //i wanna create a variable that checks whether openset is in closed set and pick neighbours that arent in closed set
			opensetArc = openset[0]->get_arcs();
			opensetchecker = closedsetchecker(closedset, openset[0]);
			temporaryPointer = NULL;
			//this is fine i need the neighbours
			for (int f = 0; f != opensetArc.size(); f++)
			{
				size1 = (opensetArc.size() - 1);
				if (opensetchecker == true)
				{
					if (transport == "Foot")
					{
						minval = opensetArc[f]->get_distance();

						for (int m = 0; m != temporaryDistances.size(); m++)
						{
							if (temporaryDistances[m] < minval)
								minval = temporaryDistances[m];
							
						}

						if (opensetArc[f]->getStart() != openset[0]->getRef())
						{
							NeighbourRef.push_back(opensetArc[f]->get_node1());
							minval = opensetArc[f]->get_distance();
							//stores arc distances in tempdist vector
							temporaryDistances.push_back(opensetArc[f]->get_distance());
						}
						else if (opensetArc[f]->getEnd() != openset[0]->getRef())
						{
							NeighbourRef.push_back(opensetArc[f]->get_node());
							temporaryDistances.push_back(opensetArc[f]->get_distance());
						}

						//pick arc with least distance
						if (opensetArc[f]->get_distance() < minval)  //HEEEEERRRRRRRRRRRRR
						{
							temporaryPointer = opensetArc[f];
						}
					}
					else if (transport == "Bike")//all arcs except foot
					{
						if (opensetArc[f]->getMode() != "Foot")
						{
							minval = opensetArc[f]->get_distance();

							for (int m = 0; m != temporaryDistances.size(); m++)
							{
								if (temporaryDistances[m] < minval)
									minval = temporaryDistances[m];
							}

							if (opensetArc[f]->getStart() != openset[0]->getRef())
							{
								NeighbourRef.push_back(opensetArc[f]->get_node1());
								minval = opensetArc[f]->get_distance();
								//stores arc distances in tempdist vector
								temporaryDistances.push_back(opensetArc[f]->get_distance());
							}
							else if (opensetArc[f]->getEnd() != openset[0]->getRef())
							{
								NeighbourRef.push_back(opensetArc[f]->get_node());
								temporaryDistances.push_back(opensetArc[f]->get_distance());
							}

							//pick arc with least distance
							if (opensetArc[f]->get_distance() < minval) //HERRRRRRRREEEEEEEEEEEEE
							{
								temporaryPointer = opensetArc[f];
							}
						}

					}
					else if (transport == "Car")//car or bus or ship
					{
						if ((opensetArc[f]->getMode() == transport)||(opensetArc[f]->getMode() == "Bus")||(opensetArc[f]->getMode() == "Ship"))
						{
							minval = opensetArc[f]->get_distance();

							for (int m = 0; m != temporaryDistances.size(); m++)
							{
								if (temporaryDistances[m] < minval)
									minval = temporaryDistances[m];
							}

							if (opensetArc[f]->getStart() != openset[0]->getRef())
							{
								NeighbourRef.push_back(opensetArc[f]->get_node1());
								minval = opensetArc[f]->get_distance();
								//stores arc distances in tempdist vector
								temporaryDistances.push_back(opensetArc[f]->get_distance());
							}
							else if (opensetArc[f]->getEnd() != openset[0]->getRef())
							{
								NeighbourRef.push_back(opensetArc[f]->get_node());
								temporaryDistances.push_back(opensetArc[f]->get_distance());
							}

							//pick arc with least distance
							if (opensetArc[f]->get_distance() <= minval) //hereeeeeeeeeeeee
							{
								temporaryPointer = opensetArc[f];
							}
						}
					}
					else if ((transport == "Rail") || (transport == "Ship"))
					{
						if (opensetArc[f]->getMode() == transport)
						{
							minval = opensetArc[f]->get_distance();

							for (int m = 0; m != temporaryDistances.size(); m++)
							{
								if (temporaryDistances[m] < minval)
									minval = temporaryDistances[m];
							}

							if (opensetArc[f]->getStart() != openset[0]->getRef())
							{
								NeighbourRef.push_back(opensetArc[f]->get_node1());
								minval = opensetArc[f]->get_distance();
								//stores arc distances in tempdist vector
								temporaryDistances.push_back(opensetArc[f]->get_distance());
							}
							else if (opensetArc[f]->getEnd() != openset[0]->getRef())
							{
								NeighbourRef.push_back(opensetArc[f]->get_node());
								temporaryDistances.push_back(opensetArc[f]->get_distance());
							}

							//pick arc with least distance
							if (opensetArc[f]->get_distance() <= minval) //hereeeeeeeeeeeeee
							{
								temporaryPointer = opensetArc[f];
							}
						}
					}
					else if (transport == "Bus")
				    {
					if ((opensetArc[f]->getMode() == transport)|| (opensetArc[f]->getMode() == "Ship"))
					  {
						minval = opensetArc[f]->get_distance();

						for (int m = 0; m != temporaryDistances.size(); m++)
						{
							if (temporaryDistances[m] < minval)
								minval = temporaryDistances[m];
						}

						if (opensetArc[f]->getStart() != openset[0]->getRef())
						{
							NeighbourRef.push_back(opensetArc[f]->get_node1());
							minval = opensetArc[f]->get_distance();
							//stores arc distances in tempdist vector
							temporaryDistances.push_back(opensetArc[f]->get_distance());
						}
						else if (opensetArc[f]->getEnd() != openset[0]->getRef())
						{
							NeighbourRef.push_back(opensetArc[f]->get_node());
							temporaryDistances.push_back(opensetArc[f]->get_distance());
						}

						//pick arc with least distance
						if (opensetArc[f]->get_distance() <= minval) //hereeeeeeeeeeee
						{
							temporaryPointer = opensetArc[f];
						}
					  }
                    }
				}
				
				else if(opensetchecker==false)
				{

					tempArcSelector = Checker(opensetArc[f], closedset);
					if (transport == "Foot")
					{
						if ((tempArcSelector == true))
							tempArcSelector = false;
						if ((tempArcSelector == true))
							temporaryPointer = opensetArc[f];
						if ((tempArcSelector == false) && (f == size1) && (temporaryPointer == NULL))
							correctArc = false;
					}
					else if (transport == "Bike") //everything except foot
					{
						if ((tempArcSelector == true) && (opensetArc[f]->getMode() == "Foot"))
							tempArcSelector = false;
						if ((tempArcSelector == true))
							temporaryPointer = opensetArc[f];
						if ((tempArcSelector == false) && (f == size1) && (temporaryPointer == NULL))
							correctArc = false;
					}
					else if (transport == "Car")
					{
						if ((tempArcSelector == true) && ((opensetArc[f]->getMode() == "Foot" )||(opensetArc[f]->getMode() == "Bike") || (opensetArc[f]->getMode() == "Rail"))) //wont take
							tempArcSelector = false;
						if ((tempArcSelector == true) && ((opensetArc[f]->getMode() == transport )||(opensetArc[f]->getMode() == "Bus") ||(opensetArc[f]->getMode() == "Ship")))//will take
							temporaryPointer = opensetArc[f];
						if ((tempArcSelector == false) && (f == size1) && (temporaryPointer == NULL))
							correctArc = false;
					}
					else if (transport == "Bus")//bus ship
					{
						if ((tempArcSelector == true) && ((opensetArc[f]->getMode() == "Foot" )||(opensetArc[f]->getMode()== "Bike")||(opensetArc[f]->getMode()== "Rail")||(opensetArc[f]->getMode()=="Car"))) //wont take
							tempArcSelector = false;
						if ((tempArcSelector == true) && ((opensetArc[f]->getMode() == "Bus") ||(opensetArc[f]->getMode()== "Ship")))//will take
							temporaryPointer = opensetArc[f];
						if ((tempArcSelector == false) && (f == size1) && (temporaryPointer == NULL))
							correctArc = false;
					}
					else if ((transport == "Rail")||( transport=="Ship"))
					{
						if ((tempArcSelector == true) && (opensetArc[f]->getMode() != transport))
							tempArcSelector = false;
						if ((tempArcSelector == true) && (opensetArc[f]->getMode() == transport))
							temporaryPointer = opensetArc[f];
						if ((tempArcSelector == false) && (f == size1) && (temporaryPointer == NULL))
							correctArc = false;
					}
					
				}

				
			}
			//clear distances vector
			temporaryDistances.clear();
			//what to do if both nodes in temporary pointer are in my closed set?..i need to check fisrt. How? Fist thing, temporary pointer contains 2 nodes. i need to check for both nodes
			//now to assign value to current
			//for first node(forget dis one you solved this issue)
			if (closedset.size() == 0)
			{
				current = openset[0];
				refe = current->getRef();
				_outFile << refe << endl;
				closedset.push_back(current);
				size1 = (closedset.size() - 1);
				openset = openset_eraser(openset, current);
				if (temporaryPointer == NULL)
					break;
				if (temporaryPointer->get_node() != current)
				{
					current = temporaryPointer->get_node();
				}

				else if (temporaryPointer->get_node1() != current)
				{
					current = temporaryPointer->get_node1();
				}
				 if (current == NULL)
				{
					break;
				}
				openset.push_back(current);	
			}
			//what is my current? i want current to fist be my start node but when i go onwards i want current to be all ways pointing towards my previous neighbour nodes
			//we are now tryna solve dis nibba over here
			else if (closedset.size() > 0)
			{
				//  i want to change the position of my current to be always pointing towards my previous neighbour node and should not be the same as in my closed set 
				if(temporaryPointer!=NULL)
				correctArc = Checker(temporaryPointer, closedset);

				if (correctArc == true) //that arc/neighbour has not been visited
				{
					//I WANT TO CHECK IF BOTH NODES CONNECTING TO MY ARC ARE IN MY CLOSED SET. ONE OF THEM PROBS ISNT AND THAT WILL CONTAIN ALL DAT SHIT AFTER THE IF STATEMENT
					//WHAT IS MY ARC? MY TEMPORARY POINTER. THEREFORE TEST TEMPORARY POINTER DESTINATION NODES
					Node* h = new Node();
					h = temporaryPointer->get_node1();
					Node* v = new Node();
					v = temporaryPointer->get_node();
					//pick non visited neighbour
					tempPointerTest1 = closedsetchecker(closedset, h);
					tempPointerTest = closedsetchecker(closedset, v);
					if (tempPointerTest == true)
					{
						        closedset.push_back(v);
								refe = current->getRef();
								_outFile << refe << endl;
								openset = openset_eraser(openset, current);
								current = v;
								openset.push_back(current);	
					}
					else if (tempPointerTest1 == true)
					{
						  closedset.push_back(h);
						  refe = current->getRef();
						  _outFile << refe << endl;
						  openset = openset_eraser(openset, current);
						  current = h;
						  openset.push_back(current); 
					}

						size1 = (closedset.size() - 1);
					
				}
				else if (correctArc == false)
				{
					//i have visited this node pick next unvisited dont matter the distance. if all been visited? back track to previous
					NeighbourRef.clear();
					//Node * j = new Node;
					//std::shared_ptr<Node> previous(backtracker(closedset, current));
					previous = backtracker(closedset, current);
					refe = previous->getRef();
					_outFile << refe << endl;
					arc_temp = previous->get_arcs();

					//loop start
					for (int x = 0; x != arc_temp.size(); x++)
					{
						if (transport == "Foot")
						{
							if (arc_temp[x]->getStart() != previous->getRef())
								NeighbourRef.push_back(arc_temp[x]->get_node1());
							else if (arc_temp[x]->getEnd() != previous->getRef())
								NeighbourRef.push_back(arc_temp[x]->get_node());
						}
						else if ((transport == "Ship" )||(transport== "Rail"))
						{
							if ((arc_temp[x]->getMode() == transport))
							{
								if (arc_temp[x]->getStart() != previous->getRef())
									NeighbourRef.push_back(arc_temp[x]->get_node1());
								else if (arc_temp[x]->getEnd() != previous->getRef())
									NeighbourRef.push_back(arc_temp[x]->get_node());
							}
						}
						else if (transport == "Bike")
						{
							if ((arc_temp[x]->getMode() != "Foot"))
							{
								if (arc_temp[x]->getStart() != previous->getRef())
									NeighbourRef.push_back(arc_temp[x]->get_node1());
								else if (arc_temp[x]->getEnd() != previous->getRef())
									NeighbourRef.push_back(arc_temp[x]->get_node());
							}
						}
						else if (transport == "Car")
						{
							if ((arc_temp[x]->getMode() == transport )||(arc_temp[x]->getMode() == "Ship")||(arc_temp[x]->getMode() == "Bus"))
							{
								if (arc_temp[x]->getStart() != previous->getRef())
									NeighbourRef.push_back(arc_temp[x]->get_node1());
								else if (arc_temp[x]->getEnd() != previous->getRef())
									NeighbourRef.push_back(arc_temp[x]->get_node());
							}
						}
						else if (transport == "Bus")
						{
							if ((arc_temp[x]->getMode() == transport || "Ship"))
							{
								if (arc_temp[x]->getStart() != previous->getRef())
									NeighbourRef.push_back(arc_temp[x]->get_node1());
								else if (arc_temp[x]->getEnd() != previous->getRef())
									NeighbourRef.push_back(arc_temp[x]->get_node());
							}
						}


						
					}
					for (int x = 0; x != NeighbourRef.size(); x++)
					{
						foundNonDuplicate = closedsetchecker(closedset, NeighbourRef[x]);
						if (foundNonDuplicate == true) //if no duplicates found value is added to closed set
						{
							openset = openset_eraser(openset, current);
							current = NeighbourRef[x];
							openset.push_back(current);
							//clear open set
							//set current as open set

							refe = current->getRef();
							_outFile << refe << endl;
							//closedset.push_back(current);
							
							break;
						}
						else if (foundNonDuplicate == false)
						{
							refe = current->getRef();
							openset = openset_eraser(openset, current);
							current = previous;
							refe = current->getRef();
							openset.push_back(current);
							
						}

					}
					//checks if duplicate found in closed set
					
					
				}

			}

			size1 = (closedset.size() - 1);

			if (current == end)
			{
				//_outFile << refe <<"\n" << endl;
				refe = current->getRef();
				_outFile << refe << "\n" << endl;
				//previous = NULL;
				current = NULL;
				NeighbourRef.clear();
				openset.clear();
				closedset.clear();
				return true;
				break;
			}
			/*if (current != end)
				closedset.push_back(current);*/
			
			NeighbourRef.clear();
	}
   
    if (openset.size() == NULL)
	{
		_outFile << "FAIL \n" << endl;
	}
	
	return true;
	}	
	
	

}



bool Navigation::BuildNetwork(const string &fileNamePlaces, const string &fileNameLinks)const
{
	fstream finPlaces(fileNamePlaces);
	fstream finLinks(fileNameLinks);

	char set_p[255];
	char newline_delimiter[255];
	string setstr, mode,name;
	double x, y, east, north;
	int ref, start, end;

	if (finPlaces.good() && finLinks.good())
	{

		//reads in places
		while (!finPlaces.eof())
		{
			finPlaces.getline(set_p, 255);
			setstr = set_p;
			splitter_1 = splitter(setstr, ',');
			name = splitter_1[0];
			ref = std::stoi(splitter_1[1]);        //place reference
			x = std::stod(splitter_1[2].c_str());   //x value
			y = std::stod(splitter_1[3].c_str());    //y value
			


			LLtoUTM(x, y, north, east); //convert x and y val to northings and eastings

			Node* const  places = new Node(name, ref, north, east);

			//destinys.clear();
			MyNodes.push_back(places);  //values pushed back to class vector
			splitter_1.clear(); //clears splitter function variables
			tokens.clear();  //clears splitter function variables
		}

		//reads in arcs
		while (finLinks >> setstr)
		{
			splitter_1 = splitter(setstr, ',');
			start = std::stoi(splitter_1[0]);
			end = std::stoi(splitter_1[1]);        //place reference
			mode = splitter_1[2];   //x value
			
			Arc * const links = new Arc(start, end, mode);

			//looks for destination node
			for (int x = 0; x != MyNodes.size(); x++)
			{
				if (end == MyNodes[x]->getRef())
				{
					Node *p = new Node;
					p = MyNodes[x];
					links->setNode(p);
				}
				else if (start == MyNodes[x]->getRef())
				{
					Node *_s = new Node;
					_s = MyNodes[x];
					links->setNode1(_s);
				}
			}
			MyArcs.push_back(links);
			splitter_1.clear(); //clears splitter function variables
			tokens.clear();  //clears splitter function variables
		}

		//links arcs with nodes
		for (int i = 0; i != MyNodes.size(); i++) //pernode
		{
			for (int j = 0; j != MyArcs.size(); j++) // i wanna get all its arcs
			{
				if (MyArcs[j]->getStart() == MyNodes[i]->getRef() || MyArcs[j]->getEnd() == MyNodes[i]->getRef())
				{
					Arc *tp = new Arc;
					tp = MyArcs[j];
					arc_temp.push_back(tp);
					MyNodes[i]->set_arc(arc_temp);
				}
			}
			arc_temp.clear();
		}
		return true;
	}
	else if (finPlaces.fail() || finLinks.fail())
		return false;
}

