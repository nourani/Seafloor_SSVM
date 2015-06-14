/*
 * Hierarchical.hpp
 *
 *  Created on: Jan 30, 2014
 *      Author: navid
 */

#ifndef HIERARCHICAL_HPP_
#define HIERARCHICAL_HPP_

#include <iostream>
#include <vector>
#include <assert.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

#include "Node.hpp"
//#include "Pair.hpp"
using namespace std;

struct Pair {
public:
	int v1;
	int v2;
	Pair(int _v1, int _v2) :
			v1(_v1), v2(_v2) {
		;
	}
};

class Hierarchy {
public:
	vector<Node> nodes;
	vector<Pair> pairs;

	int numLevels;
public:
	Hierarchy() :
			numLevels(-1) {
		;
	}

	/**
	 * Node addition/deletion
	 */
	bool addNode(void);
	bool addNode(Node n);
	bool addNodes(unsigned int num);

	/**
	 * Node connection manipulation
	 */
	bool connectParentChield(int p, int c);
	void reconnect(void);
	void removeConnections(void);

	void setNodeNumbers(void);
	void setNodeLevels(void);

	/**
	 * GET functions
	 */
	vector<int> getNodeParents(int);
	int getCommonParent(int n1, int n2);
	int getDistToCommonParent(int n1, int n2);
	int getDistBetweenNodes(int n1, int n2);
	void getStats(int, int, int *, int *, int *, int *);
	void getStatsMod(int, int, int *, int *, int *, int *);
	string getBinaryString(int, bool weighted=false);
	/**
	 * UI
	 */
	void printHierarchy(void);
};

#endif /* HIERARCHICAL_HPP_ */
