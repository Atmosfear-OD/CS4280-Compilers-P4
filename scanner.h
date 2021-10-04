/*
 * scanner.h by Pascal Odijk 4/25/2021
 * P3 CS4280 Professor Hauschild
 * 
 * This file contains the function protoypes for the scanner, findColumn, and lookup in scanner.cpp. Additionally, the structures for lookupChar and lookupToken are specified in this file but the actual
 * characters and tokens declarations are found in scanner.cpp. 
 *
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "token.h"

struct lookupChar {
	char value;
	int key;
};

struct lookupToken {
	int key;
	tokenID tkEnum;
};

// Scanner prototype
token scanner(ifstream& ifile, int lineNum);

// Prototype to lookup column on FSA table
int findColumn(char nextChar);

// Protoype to map token and keywords
token lookup(int value, string searchString);

#endif
