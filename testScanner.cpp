/*
 * testScanner.cpp by Pascal Odijk 4/25/2021
 * P3 CS4280 Professor Hauschild
 * 
 * This file contains the testScanner function which is the driver for the actual scanner in scanner.cpp. The function testScanner passes the scanner one character at a time, once tokenized,
 * they are returned to the testScanner and are then printed in the format {tokenID, instance, and line number}. Additionally, testScanner handles the error if an identifier begins with a capital letter as well 
 * as if a token is specified as an error in the scanner. The tokenNames array holds the string representation for the tokenID and is displayed in the token triplet. 
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "testScanner.h"
#include "scanner.h"

using namespace std;

// Token names to output
string tokenNames[] = {
	"End of File",
	"Error",
	"Identifier",
	"Integer",
	"Begin Keyword",
	"End Keyword",
	"Loop Keyword",
	"Whole Keyword",
	"Void Keyword",
	"Exit Keyword",
	"Getter Keyword",
	"Outter Keyword",
	"Main Keyword",
	"If Keyword",
	"Then Keyword",
	"Assign Keyword",
	"Data Keyword",
	"Proc Keyword",
	"Equals Sign",
	"Greater Sign",
	"Greater Equal",
	"Less Sign",
	"Less Equal",
	"Double Equal",
	"Colon",
	"Colon Equal",
	"Plus Sign",
	"Minus Sign",
	"Asterisk",
	"Slash",
	"Modulus",
	"Period",
	"Open Parenthesis",
	"Close Parenthesis",
	"Comma",
	"Open Curly",
	"Close Curly",
	"Semicolon",
	"Open Bracket",
	"Close Bracket"
};

// Driver for the scanner
void testScanner(ifstream& ifile){
	token tk;
	int lineNum = 1;

	while(ifile) {
		tk = scanner(ifile, lineNum);

		if(tokenNames[tk.tkIdentifier] == "Identifier" && isalpha(tk.tkName[0])) {
			if(tk.tkName[0] == toupper(tk.tkName[0])) {
				cout << "SCANNER ERROR: invalid identifier " << tk.tkName << " at line " << tk.lineNum << "\n";
				cout << "Identifiers cannot begin with a capital letter\n";

				return;
			}
		}
		
		if(tokenNames[tk.tkIdentifier] == "Error") {
			return;
		}
		else {
				cout << "{ TokenID: " << tokenNames[tk.tkIdentifier] << ", Instance: " 
			     << tk.tkName << ", Line: " << tk.lineNum << " }\n";
		}
	}

	return;
}
