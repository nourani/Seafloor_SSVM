/*
 * HierarchicalWrapper.h
 *
 *  Created on: Feb 4, 2014
 *      Author: navid
 */

#ifndef HIERARCHICALWRAPPER_H_
#define HIERARCHICALWRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Hierarchy Hierarchy;

Hierarchy * newHierarchy();

int hierarchy_add_nodes(Hierarchy *, unsigned int);

int hierarchy_connect_parent_chield(Hierarchy *, int, int);

void hierarchy_reconnect(Hierarchy *);


int hierarchy_get_num_classes(Hierarchy *);

void hierarchy_get_binary_string(Hierarchy *, int, char *);
void hierarchy_get_binary_string_weighted(Hierarchy *, int, char *);

void hierarchy_get_stats(Hierarchy *, int, int, int *, int *, int *, int *);
int hierarchy_get_dist_to_parent(Hierarchy *, int, int);
int hierarchy_get_dist_between_nodes(Hierarchy *, int, int);

void hierarchy_print( Hierarchy * );

#ifdef __cplusplus
}
#endif

#endif /* HIERARCHICALWRAPPER_H_ */
