/*
 * printTree.cpp by Pascal Odijk 4/25/2021
 * P3 CS4280 Professor Hauschild
 *
 * This file prints the tree in preorder with an array storing the token names as strings.
 */

#include <iostream>
#include "printTree.h"

static int level = 0;

// Token names to output
string tokenName[] = {
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

// Prints the tree in preorder
void printTree(node *pTree){
        if(pTree != NULL){
		token tk = pTree->nToken;
 
		for(int i = 0 ; i < pTree->level ; i++){ cout << "  "; }
		
		//level++;
		
		cout << pTree->name << "\t";

		if(tk.tkName != "") {
			cout << "{ TokenID: " << tokenName[tk.tkIdentifier] << ", Instance: " << tk.tkName << ", Line: " << tk.lineNum << " }\n"; 
			tk = pTree->nToken2;
			if(tokenName[tk.tkIdentifier] != "End of File") {
				for(int i = 0 ; i < level ; i++){ cout << "  "; }			
				cout << "\t\t{ TokenID: " << tokenName[tk.tkIdentifier] << ", Instance: " << tk.tkName << ", Line: " << tk.lineNum << " }\n";
				tk = pTree->nToken3; 
				if(tokenName[tk.tkIdentifier] != "End of File") {
					for(int i = 0 ; i < level ; i++){ cout << "  "; }
					cout << "\t\t{ TokenID: " << tokenName[tk.tkIdentifier] << ", Instance: " << tk.tkName << ", Line: " << tk.lineNum << " }\n";
					tk = pTree->nToken4;
					if(tokenName[tk.tkIdentifier] != "End of File") {
						for(int i = 0 ; i < level ; i++){ cout << "  "; }
                                        	cout << "\t\t{ TokenID: " << tokenName[tk.tkIdentifier] << ", Instance: " << tk.tkName << ", Line: " << tk.lineNum << " }\n";
						tk = pTree->nToken5;
						if(tokenName[tk.tkIdentifier] != "End of File") {
							for(int i = 0 ; i < level ; i++){ cout << "  "; }
                                        		cout << "\t\t{ TokenID: " << tokenName[tk.tkIdentifier] << ", Instance: " << tk.tkName << ", Line: " << tk.lineNum << " }\n";
						}
					}
				}		
			}
		}

                cout << "\n";
		
		// Process children
                printTree(pTree->child1);
                printTree(pTree->child2);
		printTree(pTree->child3);
		printTree(pTree->child4);
        }
}
