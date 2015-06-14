/*
 * HierarchicalWrapper.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: navid
 */

#include "Hierarchy.hpp"
#include "HierarchicalWrapper.h"

extern "C" {
Hierarchy * newHierarchy() {
	return new Hierarchy();
}

int hierarchy_add_nodes(Hierarchy* h, unsigned int num) {
	return h->addNodes(num);
}

int hierarchy_connect_parent_chield(Hierarchy * h, int p, int c) {
	return h->connectParentChield(p, c);
}

void hierarchy_reconnect(Hierarchy * h) {
	h->reconnect();
}

int hierarchy_get_num_classes(Hierarchy *h) {
	return h->nodes.size();
}

void hierarchy_get_binary_string(Hierarchy * h, int n, char * str) {
	string s = h->getBinaryString(n);
	strcpy(str, s.c_str());
}
void hierarchy_get_binary_string_weighted(Hierarchy * h, int n, char * str) {
	string s = h->getBinaryString(n, true);
	strcpy(str, s.c_str());
}

void hierarchy_get_stats(Hierarchy * h, int n1, int n2, int *tp, int *fp,
		int *fn, int *tn) {
	h->getStatsMod(n1, n2, tp, fp, fn, tn);
}
int hierarchy_get_dist_to_parent(Hierarchy * h, int n1, int n2) {
	return h->getDistToCommonParent(n1, n2);
}
int hierarchy_get_dist_between_nodes(Hierarchy * h, int n1, int n2) {
	return h->getDistBetweenNodes(n1, n2);
}


void hierarchy_print(Hierarchy * h) {
	h->printHierarchy();
}

}

