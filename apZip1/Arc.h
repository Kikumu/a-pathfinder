#pragma once
#include "Node.h"
#include<string>
#include<vector>

using namespace std;


class Node;

//Node::AddNode();


class Arc
{
private:
	
	//vector<Node>*destination;

	int Start, End;
	string mode = " ";
	double distance;
	Node* destination_node1;
	Node* start_node;

public:
	////DEFAULT CONSTRUCTORS
	////Arc & operator=Arc x;

	Arc(const Arc &);
	Arc& operator = (const Arc &);
	Arc();
	//overload constructors
	Arc(int, int, string&);

	//destructor
	~Arc();

	//accessor minds
	int getStart()const;
	int getEnd() const;
	const string& getMode() const;
	double get_distance()const;
	Node*get_node()const; //end node
	Node*get_node1()const; //start node
	

	//mutators
	void setStart(int);
	void setEnd(int);
	/*void setMode(string&);*/
	void setdistance(double);
	void setNode(Node*); //set end node
	void setNode1(Node*); //set start node
};
