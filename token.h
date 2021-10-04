/*
 * token.h by Pascal Odijk 4/29/2021
 * P4 CS4280 Professor Hauschild
 *
 * This file contains the enumeration and structure of the tokens.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

using namespace std;

// Enumeration of tokens
enum tokenID {
	// EOF and error tokens
	eofTk, errorTk,

	// Identifiers and integers tokens
	idTk, numTk,
	
	// Keyword tokens
	beginTk, endTk, loopTk, wholeTk, voidTk, exitTk, 
	getterTk, outterTk, mainTk, ifTk, thenTk, assignTk, 
	dataTk, procTk, 

	// Operator and delimiter tokens
	equalsTk, greatTk, great_eqTk, lessTk, less_eqTk, double_equalsTk,
	colonTk, colon_equalsTk, plusTk, minusTk, asteriskTk, 
	slashTk, modulusTk, periodTk, open_parTk, close_parTk,
	commaTk, open_curlTk, close_curlTk, semicolonTk, 
	open_bracketTk, close_bracketTk
};

// Token structure
struct token {
	tokenID tkIdentifier;
	string tkName;
	int lineNum;
	int scope;
};

#endif

