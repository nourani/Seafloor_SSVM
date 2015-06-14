/*
 * Node.hpp
 *
 *  Created on: Jan 30, 2014
 *      Author: navid
 */

#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

#ifndef NODE_HPP_
#define NODE_HPP_

class Node {

public:
	Node * parent;
	vector<Node *> children;

	string name;
	vector<string> labels;
	int num;
	int lvl;

	Node() : parent(NULL), num(-1), lvl(-1) {
		;
	}
	Node( Node * p, string n, int num, int lvl ) : parent(p), name(n), num(num), lvl(lvl) {
		;
	}

	void clear( void );

	std::string toString(void) const;

};


#endif /* NODE_HPP_ */
