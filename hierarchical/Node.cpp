/*
 * Node.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: navid
 */

#include "Node.hpp"

void Node::clear(void) {
	this->parent = NULL;
	children.clear();
	name = "";
	labels.clear();
	num=-1;
	lvl=-1;
}

std::string Node::toString(void) const {
	char num[8];
	char lvl[8];
	sprintf(num, "%d", this->num);
	sprintf(lvl, "%d", this->lvl);


	string s = name + "\n" +
			"\tNum=" + (string)num + "\n" +
			"\tLvl=" + (string)lvl + "\n" +
			"\tparent=" + (parent == NULL ? "None" : parent->name) + "\n" +
			"\tchildren=" + (children.size() == 0 ? "None" : "");
	for (int i = 0; i < children.size(); i++) {
		s += (children.at(i))->name + ";";
	}
	return s;
}

