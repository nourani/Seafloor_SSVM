/*
 * NodeTest.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: navid
 */

#include <iostream>
#include <vector>

#include "Hierarchy.hpp"

using namespace std;

int main(int argc, char ** argv) {

	// Create a hieararchy of 5 nodes
	Hierarchy h;
	h.addNodes(20);
	// Connect nodes 1-2 to the root node. Connect nodes 3-4 to node 1
	h.connectParentChield( 0, 1);
	h.connectParentChield( 0, 4);
	h.connectParentChield( 1, 2);
	h.connectParentChield( 1, 3);
	h.connectParentChield( 4, 5);
	h.connectParentChield( 4, 6);
	h.connectParentChield( 4, 7);
	h.connectParentChield( 4, 8);
	h.connectParentChield( 4, 9);
	h.connectParentChield( 4,10);
	h.connectParentChield(10,11);
	h.connectParentChield(10,14);
	h.connectParentChield(10,17);
	h.connectParentChield(11,12);
	h.connectParentChield(11,13);
	h.connectParentChield(14,15);
	h.connectParentChield(14,16);
	h.connectParentChield(17,18);
	h.connectParentChield(17,19);
	// Set node numbers and levels
	h.reconnect();
//	h.setNodeNumbers();
//	h.setNodeLevels();

	// See the connections between the nodes
	h.printHierarchy();

	int n1 = 19;
	int n2 = 3;
	cout << "n1=" << n1 << " : \'" << h.getBinaryString(n1) << "\' => \'" <<
			h.getBinaryString(n1,true) << "\'" << endl;
	cout << "n2=" << n2 << " : \'" << h.getBinaryString(n2) << "\' => \'" <<
				h.getBinaryString(n2,true) << "\'" << endl;
	cout << endl << "Parents to node " << n1 << endl;
	vector<int> ps = h.getNodeParents( n1 );
	for( int i = 0; i < ps.size(); i++ ) {
		cout << ps[i] << ", ";
	}
	cout << endl;

	cout << endl << "Parents to node " << n2 << endl;
	ps = h.getNodeParents( n2 );
	for( int i = 0; i < ps.size(); i++ ) {
		cout << ps[i] << ", ";
	}
	cout << endl;

	cout << endl << "Common parents between " << n1 << " and " << n2 << endl;
	cout << h.getCommonParent(n1, n2) << endl;

	int tp=0,fp=0,fn=0,tn=0;
	cout << endl << "Stats between " << n1 << " and " << n2 << endl;
	h.getStats(n1,n2,&tp,&fp,&fn,&tn);
	cout << "tp/fp/fn/tn = " << tp << "/" << fp << "/" << fn << "/" << tn << endl;

	cout << endl << "dist to parent = " << h.getDistToCommonParent(n1, n2) << endl;
	cout << "dist between nodes = " << h.getDistBetweenNodes(n1, n2) << endl;

	return 0;
}

