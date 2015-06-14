/*
 * Hierarchy.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: navid
 */

#include "Hierarchy.hpp"

/**
 * Node addition/deletion
 */
bool Hierarchy::addNode(void) {
	Node n;
	char str[16];
	sprintf(str, "Node %ld", nodes.size());
	n.name = (string) str;
	return addNode(n);
}
bool Hierarchy::addNode(Node n) {
	nodes.push_back(n);

	// Because we are maintaining pointers within an STL::vector, every time
	// 	the vector is modifies we must update the points.
	reconnect();
	return true;
}
bool Hierarchy::addNodes(unsigned int num) {
	Node n;
	for (unsigned int i = 0; i < num; i++) {
		if (!addNode())
			return false;
	}
	return true;
}

/**
 * Node connection manipulation
 */
bool Hierarchy::connectParentChield(int p, int c) {
	if (p < 0 || c < 0 || p >= nodes.size() || c >= nodes.size())
		return false;

	bool exists = false;
	for (int i = 0; i < pairs.size(); i++) {
		if (pairs[i].v1 == p && pairs[i].v2 == c)
			exists = true;
	}
	if (!exists)
		pairs.push_back(Pair(p, c));

	nodes.at(c).parent = &(nodes.at(p));
	nodes.at(p).children.push_back(&(nodes.at(c)));

	return true;
}

void Hierarchy::reconnect(void) {
	removeConnections();
	for (int i = 0; i < pairs.size(); i++)
		connectParentChield(pairs[i].v1, pairs[i].v2);
	setNodeNumbers();
	setNodeLevels();
}

void Hierarchy::removeConnections(void) {
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i].parent = NULL;
		nodes[i].children.clear();
		nodes[i].lvl = -1;
		nodes[i].num = -1;
	}
}

void Hierarchy::setNodeNumbers(void) {
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i].num = i;
	}
}
void Hierarchy::setNodeLevels(void) {
	int lvl = 0;
	// Set root node(s)
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].parent == NULL)
			nodes[i].lvl = lvl;
	}
	bool changed = false;
	do {
		changed = false;
		for (int i = 0; i < nodes.size(); i++) {
			if (nodes[i].parent && nodes[i].parent->lvl == lvl) {
				nodes[i].lvl = lvl + 1;
				changed = true;
			}
		}
		lvl++;
	} while (changed);
	this->numLevels = lvl;
}

/**
 * GETs
 */
vector<int> Hierarchy::getNodeParents(int num) {
	vector<int> ps;
	Node * n = &(nodes.at(num));
	for (int i = 0; i <= nodes.at(num).lvl; i++) {
		if (!n->parent)
			break;
		ps.push_back(n->parent->num);
		n = n->parent;
	}
	return ps;
}

int Hierarchy::getCommonParent(int n1, int n2) {
	vector<int> p1 = getNodeParents(n1);
	vector<int> p2 = getNodeParents(n2);

	for (int i = 0; i < p1.size(); i++)
		for (int j = 0; j < p2.size(); j++)
			if (p1[i] == p2[j] || p1[i] == n2 || p2[j] == n1)
				return p1[i];
	return -1;
}

int Hierarchy::getDistToCommonParent(int n1, int n2) {
	int d;
	vector<int> p1 = getNodeParents(n1);
	vector<int> p2 = getNodeParents(n2);
//	p1.insert(p1.begin(), n1);
//	p2.insert(p2.begin(), n2);
	int cp = getCommonParent(n1, n2);
	for (d = 0; d < p1.size(); d++)
		if (p1[d] == cp)
			break;
	assert(d >= 0);
	return d;
}

int Hierarchy::getDistBetweenNodes(int n1, int n2) {
	int d = 0;
	if( n1 == n2 )
		return d;

	vector<int> p1 = getNodeParents(n1);
	vector<int> p2 = getNodeParents(n2);
	p1.insert(p1.begin(), n1);
	p2.insert(p2.begin(), n2);
	int cp = getCommonParent(n1, n2);
	for (int i = 0; i < p1.size(); i++, d++) {
		if (p1[i] == cp)
			break;
	}
	for(int i = 0; i < p2.size(); i++, d++ ) {
		if( p2[i] == cp )
			break;
	}
	assert(d >= 0);
	return d;
}

void Hierarchy::getStats(int n1, int n2, int * tp, int *fp, int *fn, int *tn) {
	// Get the ground truth (p1) and prediction (p2) nodes
	vector<int> p1 = getNodeParents(n1);
	vector<int> p2 = getNodeParents(n2);
	p1.push_back(n1);
	p2.push_back(n2);

	// For the ground truth, every prediction is the same is a TP. Every
	//	one that is missing is FN.
	for (int i = 0; i < p1.size(); i++) {
		bool foundP1 = false;
		for (int j = 0; j < p2.size(); j++) {
			if (p1[i] == p2[j]) {
				(*tp)++;
				foundP1 = true;
			}
		}
		if (!foundP1) {
			(*fn)++;
		}
	}
	// The number of predictions minus the correct predictions is the FPs
	*fp = (int)p2.size() - (*tp);
	// The total number of nodes, -TP-FN-FP is TNs
	*tn = (int)nodes.size() - (*tp) - *fp - *fn;

//	cout << "stats for " << n1 << " vs " << n2 << ": tp/fp/fn/tn = " <<
//					*tp << "/" << *fp << "/" << *fn << "/" << *tn << endl;
//	assert( *tp+*fp+*fn+*tn == nodes.size() );
}


/**
 * Calcualte modified hierarchical stats
 */
void Hierarchy::getStatsMod(int n1, int n2, int * tp, int *fp, int *fn, int *tn) {
	// Get the ground truth (p1) and prediction (p2) nodes
	vector<int> p1 = getNodeParents(n1);
	vector<int> p2 = getNodeParents(n2);

	// Check if n1 appears as a parent to n2
	bool isChild = false;
	for(int i = 0; i < p2.size(); i++ ) {
		if( n1 == p2[i] ) {
			isChild = true;
			break;
		}
	}

	// now push back the nodes themselves
	p1.push_back(n1);
	p2.push_back(n2);


	// For the ground truth, every prediction is the same is a TP. Every
	//	one that is missing is FN.
	for (int i = 0; i < p1.size(); i++) {
		bool foundP1 = false;
		for (int j = 0; j < p2.size(); j++) {
			if (p1[i] == p2[j]) {
				(*tp)++;
				foundP1 = true;
			}
		}
		if (!foundP1) {
			(*fn)++;
		}
	}
	// In the case that the prediction is at a deeper level than the ground truth
	//	we do not count these as FPs.
	if( !isChild ) {
		// The number of predictions minus the correct predictions is the FPs
		*fp = (int)p2.size() - (*tp);
	}
	// The total number of nodes, -TP-FN-FP is TNs
	*tn = (int)nodes.size() - (*tp) - *fp - *fn;

//	cout << "stats for " << n1 << " vs " << n2 << ": tp/fp/fn/tn = " <<
//					*tp << "/" << *fp << "/" << *fn << "/" << *tn << endl;
//	assert( *tp+*fp+*fn+*tn == nodes.size() );
}

string Hierarchy::getBinaryString(int n, bool weighted) {
	
	char * s = new char[nodes.size() + 1];
	memset((void *) s, '0', nodes.size());

	vector<int> p = getNodeParents(n);
	p.push_back(n);
	
	for (int i = 0; i < p.size(); i++) {
		if (weighted) {
			s[p[i]] = '0' + (numLevels - nodes[p[i]].lvl);
		} else {
			s[p[i]] = '1';
		}
	}
	s[nodes.size()] = '\0';
	return (string) s;
}
/**
 * UI
 */
void Hierarchy::printHierarchy(void) {
	cout << nodes.size() << " nodes..." << endl;
	for (int i = 0; i < nodes.size(); i++)
		cout << nodes.at(i).toString() << endl;
}

