/*
 * scanner.cpp by Pascal Odijk 4/25/2021
 * P3 CS4280 Professor Hauschild
 * 
 * This file contains the FSA table for the scanner to properly identify the final states for identifiers/digits/characters. The arrays finalCharLookup, finalTokenLookup, and finalKeywordLookup store the 
 * valid values for the characters, tokens, and keywords respectfully. These arrays are used by the functions findColumn and lookup to identify what are valid characters, what column to look at in the FSA table,
 * and what strings are keywords and not identifiers. The scanner keeps collecting characters until a final state is recognized and thus returned to the testScanner to output as a token. Comments (beginning
 * and ending with a $$) are skipped, but will output an error if a comment is never closed. An error will aslo display if an invalid character or identifier is encountered. An invalid identifier is one
 * that begins with a capital letter or is longer than eight characters. 
 */


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "scanner.h"

using namespace std;

// FSA Table
int FSATable[26][24] = {
	  // WS     L     D     =     <     >     _     :     +     -     *     /     %     .     (     )     ,     {     }     ;     [     ]   EOF 
/* s0 */  {   0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,   -1},
/* s1 */  {1000,    1,    1, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
/* s2 */  {1001, 1001,    2, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001},
/* s3 */  {1002, 1002, 1002,   22,   23,   24, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002},
/* s4 */  {1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005},
/* s5 */  {   9,    9,    9, 1002, 1004, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002},
/* s6 */  {1000,    1,    1, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
/* s7 */  {1008, 1008, 1008,   25, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008},
/* s8 */  {1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010},
/* s9 */  {1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011},
/* s10 */ {1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012},
/* s11 */ {1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013},
/* s12 */ {1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014},
/* s13 */ {1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015},
/* s14 */ {1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016},
/* s15 */ {1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017},
/* s16 */ {1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018},
/* s17 */ {1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019},
/* s18 */ {1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020},
/* s19 */ {1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021},
/* s20 */ {1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022},
/* s21 */ {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023},
/* s22 */ {1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007},
/* s23 */ {1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006},
/* s24 */ {1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004},
/* s25 */ {1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009},
};

// List of characters and corresponding column
lookupChar finalCharLookup[] = {
	{'=', 3}, {'<', 4}, {'>', 5}, {'_', 6}, {':', 7}, {'+', 8},
	{'-', 9}, {'*', 10}, {'/', 11}, {'%', 12}, {'.', 13},{'(', 14},
	{')', 15},{',', 16},{'{', 17},{'}', 18},{';', 19},{'[', 20},
	{']', 21}
};

// List of tokens and corresponding final state
lookupToken finalTokenLookup[] = {
	{-1, eofTk},  {-2, errorTk}, {1000, idTk}, {1001, numTk}, {1002, equalsTk}, {1003, greatTk}, {1004, great_eqTk},
	{1005, lessTk}, {1006, less_eqTk}, {1007, double_equalsTk}, {1008, colonTk}, {1009, colon_equalsTk},
	{1010, plusTk}, {1011, minusTk}, {1012, asteriskTk}, {1013, slashTk}, {1014, modulusTk}, {1015, periodTk},
	{1016, open_parTk}, {1017, close_parTk}, {1018, commaTk}, {1019, open_curlTk}, {1020, close_curlTk},
	{1021, semicolonTk}, {1022, open_bracketTk}, {1023, close_bracketTk}
};

// List of reserved keywords
token finalKeywordLookup[] = {
	{beginTk, "begin", 0}, {endTk, "end", 0}, {loopTk, "loop", 0}, {wholeTk, "whole", 0},
	{voidTk, "void", 0}, {exitTk, "exit", 0}, {getterTk, "getter", 0}, {outterTk, "outter", 0}, 
	{mainTk, "main", 0}, {ifTk, "if", 0}, {thenTk, "then", 0}, {assignTk, "assign", 0}, {dataTk, "data", 0}, {procTk, "proc", 0}
};

token scanner(ifstream& ifile, int lineN) {
	token returnTk;
	static int lineNum = lineN;
	char currChar;
	char nextChar;
	int stateColumn;
	int currentState = 0;
	int nextState = 0;
	string tkString;

	while(currentState < 1000 && currentState >= 0) {
		ifile.get(currChar);
		
		// Check if at end of file
		if(ifile.eof()) {
			returnTk.tkIdentifier = eofTk;
			returnTk.lineNum = lineNum;
			returnTk.tkName = "EOF";
			
			return returnTk;
		}
		
		// Skip comments
		if(currChar == '$') {
			int line = lineNum;
			ifile.get(currChar);
			ifile.get(currChar);
			
			ifile.get(nextChar);
			while(currChar != '$' && nextChar != '$') {
				currChar = nextChar;
				ifile.get(nextChar);
	
				// Check if comment started but never closed
				if(ifile.eof()) {
					cout << "SCANNER ERROR: comment started at line " << line << " but never closed\n";
					returnTk.tkIdentifier = errorTk;
					returnTk.lineNum = line;
					returnTk.tkName = "Error";
					
					exit(1);
					//return returnTk;
				}

				if(nextChar == '\n') {
					lineNum++;
				}
			}

			ifile.get(currChar);
			ifile.get(currChar);
		}

		
		// Set nextState
		stateColumn = findColumn(currChar);
		nextState = FSATable[currentState][stateColumn];	
		
		// Check if character is invalid
		if(stateColumn == -2) {
			cout << "SCANNER ERROR: at line " << lineNum << " invalid character: " << currChar << "\n";
			returnTk.tkIdentifier = errorTk;
			returnTk.lineNum = lineNum;
			returnTk.tkName = currChar;
			
			exit(1);
			//return returnTk;
		}
	
		if(nextState >= 1000 || nextState == -1) {
			// If EOF
			if(nextState == -1) {
				returnTk.tkIdentifier = eofTk;
				returnTk.lineNum = lineNum;
				returnTk.tkName = "EOF";

				return returnTk;
			}
			// If double character '=>', '=<', '==', ':='
			else if(nextState == 1004 || nextState == 1006 || nextState == 1007 || nextState == 1009) {
				returnTk = lookup(nextState, tkString);
				returnTk.lineNum = lineNum;
				ifile.unget();
				
				return returnTk;
			}
			else if(nextState == 1004 || nextState == 1006) {
				returnTk = lookup(nextState, tkString);
                                returnTk.lineNum = lineNum;
                                ifile.unget();
				ifile.unget();

                                return returnTk;
			}
			// If at final state 
			else if(nextState >= 1000) {
				returnTk = lookup(nextState, tkString);
				returnTk.lineNum = lineNum;
				ifile.unget();
				
				// Catch error if identifier begins with capital letter
				if(nextState == 1000) {
					 if(tkString[0] != '_' && tkString[0] == toupper(tkString[0])) {
						cout << "SCANNER ERROR: invalid identifier " << tkString << " at line " << lineNum << "\n";
						cout << "Identifiers cannot begin with a capital letter\n";
						
						exit(1);
					}
				}

				return returnTk;
			}
		}
		// Keep reading in characters if not at a final state, EOF, or error
		else {
			char value = currChar;
			
			if(!isspace(value)) {
				tkString += value;
			}			

			// Check 8 char/digit length limit
			if(tkString.length() > 8) {
				cout << "SCANNER ERROR: at line : " << lineNum << "\n";
				cout << "identifier/number length exception: " << tkString << " is greater than 8 characters/digits\n";
				returnTk.tkIdentifier = errorTk;
				returnTk.lineNum = lineNum;
				returnTk.tkName = "Error";
	
				exit(1);
				//return returnTk;
			}

			if(currChar == '\n') {
				lineNum++;
			}
			
			currentState = nextState;
		}
	}
		
	// Catch scanner failure
	returnTk.tkIdentifier = errorTk;
	returnTk.lineNum = lineNum;
	returnTk.tkName = "SCANNER ERROR: Scanner Failure\n";
	cout << "SCANNER ERROR: scanner failure\n";

	exit(1);
	//return returnTk;	
}

// Find column on FSA table
int findColumn(char nextChar) {
	if(isalpha(nextChar)) {
		return 1;
	}
	else if(isdigit(nextChar)) {
		return 2;
	}
	else if(isspace(nextChar)) {
		return 0;
	}
	else if(nextChar == EOF) {
		return 22;
	}
	else if(nextChar != EOF || !isalpha(nextChar) || !isdigit(nextChar) || !isspace(nextChar)) {
		for(int i = 0 ; i < (sizeof(finalCharLookup)/sizeof(finalCharLookup[0])) ; i++) {
			if(finalCharLookup[i].value == nextChar) {
				return finalCharLookup[i].key;
			}
		}
	}
	
	return -2;
}

token lookup(int value, string searchString) {
	token returnTk;
	
	for(int i = 0 ; i < (sizeof(finalTokenLookup)/sizeof(finalTokenLookup[0])) ; i++) {
		if(finalTokenLookup[i].key == value) {
			returnTk.tkIdentifier = finalTokenLookup[i].tkEnum;
			returnTk.tkName = searchString;
			returnTk.lineNum = 999;
		}


	}

	if(value == 1000) {
		for(int j = 0 ; j < (sizeof(finalKeywordLookup)/sizeof(finalKeywordLookup[0])) ; j++) {
			if(finalKeywordLookup[j].tkName == searchString) {
					returnTk.tkIdentifier = finalKeywordLookup[j].tkIdentifier;
					returnTk.tkName = finalKeywordLookup[j].tkName;
					returnTk.lineNum = 999;
			}
			
		}
			
	}

	return returnTk;
}
