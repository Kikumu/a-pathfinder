#include "Node.h"

Node::Node()
{
	reference = NULL;
	x_val = NULL;
	y_val = NULL;
}

Node::Node(string  &p_name, int const p_ref, double const p_xval, double const p_yval) : name(p_name), reference(p_ref), x_val(p_xval), y_val(p_yval)
{
	
}

Node::~Node()
{
}

const string& Node::getPlace() const
{
	return name;
}

int Node::getRef() const
{
	return reference;
}

double Node::get_xval() const
{
	return x_val;
}

double Node::get_yval() const
{
	return y_val;
}

const vector<Arc*> & Node::get_arcs() const
{
	return m_arcs;
}

void Node::setRef(int const  s_ref)
{
	reference = s_ref;
}

void Node::set_xval(double const set_x)
{
	x_val = set_x;
}

void Node::set_yval(double const set_y)
{
	y_val = set_y;
}

void Node::set_arc(const vector<Arc*> & const arc)
{
     m_arcs = arc;
}

//void Node::AddNode(string n, int r, double x, double y)
//{
//	p = new node;
//	p->next = NULL;
//
//	p->_plc = n;
//	p->_ref = r;
//	p->_x = x;
//	p->_y = y;
//
//	if (head != NULL)
//	{
//		current = head;
//		while (current->next != NULL)
//		{
//			current = current->next;
//		}
//		current->next = p;
//			
//	}
//	else
//	{
//		head = p;
//	}
//}

//bool Node::Search(nodePtr p, int r)
//{
//	if (p == NULL)
//	{
//		return false;
//	}
//	else if(p ->_ref== r)
//	{
//
//		return true;
//	}
//	else
//	{
//		Search(p->next, r);
//	}
//}
