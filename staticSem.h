/*
 * staticSem.h by Pascal Odijk 4/29/2021
 * P4 CS4280 Professor Hauschild
 *
 * This file contains the function prototypes used in staticSem.cpp.
 */

#ifndef STATICSEM_H
#define STATICSEM_H

#include <fstream>
#include <iostream>
#include "token.h"
#include "node.h"

void push(token stackTk);
void pop(int startScope, ofstream& outFile);
void initStack(void);

int find(string str);
int varExists(string str);

void semanticCheck(node *node, int count, ofstream& outFile);

#endif
