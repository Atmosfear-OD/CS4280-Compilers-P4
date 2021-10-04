/*
 * node.h by Pascal Odijk 4/29/2021
 * P4 CS4280 Professor Hauschild
 *
 * This file contains the strcture for the nodes within the tree.
 */

#ifndef NODE_H
#define NODE_H

#include "token.h"

using namespace std;

struct node {
	string name;
	int level;

	token nToken;
	token nToken2;
	token nToken3;
	token nToken4;
	token nToken5;
		
	node *child1;
	node *child2;
	node *child3;
	node *child4;
};

#endif
