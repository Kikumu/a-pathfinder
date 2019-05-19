#include "Arc.h"
#include"Node.h"
#include <memory>
#include <algorithm>


using namespace std;


Arc::Arc()
{
	mode = " ";
	Start = NULL;
	End = NULL;
	destination_node1 = NULL;
	distance = NULL;
	start_node = NULL;
}

Arc::Arc(int const _start, int  const _end, string& _mode) : Start(_start), End(_end), mode(_mode)
{
	destination_node1 = NULL;
	distance = NULL;
	start_node = NULL;

}

Arc::~Arc()
{
}

int Arc::getStart() const
{
	return Start;
}

int Arc::getEnd() const
{
	return End;
}

const std::string& Arc::getMode() const
{
	return mode;
}

double Arc::get_distance() const
{
	return distance;
}

Node* Arc::get_node()const
{
	return destination_node1;
}

Node * Arc::get_node1() const
{
	return start_node;
}



void Arc::setStart(int const a)
{
	Start = a;
}

void Arc::setEnd(int const b)
{
	End = b;
}

//void Arc::setMode(string c)
//{
//	mode = c;
//}

void Arc::setdistance(double const d)
{
	distance = d;
}

void Arc::setNode(Node* const n)
{
	destination_node1 = n;
}

void Arc::setNode1(Node * const s)
{
	start_node = s;
}



//nodePtr_destination Arc::finder(int end_int) const
//{
//	
//	return nodePtr_destination();
//}

//bool Node::Search(nodePtr p, int r)
//{
//	if (p == NULL)
//	{
//		return false;
//	}
//	else if (p->_ref == r)
//	{
//
//		return true;
//	}
//	else
//	{
//		Search(p->next, r);
//	}
//}
