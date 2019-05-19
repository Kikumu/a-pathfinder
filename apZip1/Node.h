#pragma once

#include<string>
#include<vector>
#include "Arc.h"
using namespace std;


class Arc;
//extern void taker(vector<vector<string>> Mystack1);



class Node
{
private:
	vector<Arc*>m_arcs;
	string name;
	int reference;
	double x_val, y_val;
	

public:
	

	//DEFAULT CONSTRUCTORS
	Node();

	//overload constructors
	Node(string&, int, double, double);

	//destructor
	~Node();

	//accessor variables. const cause we dont wanna change them
	const string& getPlace() const;
	int getRef()const;
	double get_xval() const;
	double get_yval() const;
	const vector<Arc*>& get_arcs()const;

	//mutators
	void setRef(int);
	void set_xval(double);
	void set_yval(double);
	void set_arc(const vector<Arc*>&);
	


	//searcher
	//bool Search(nodePtr, int);

	//pointers
};
