/*
 * staticSem.cpp by Pascal odijk 4/28/2021
 * P4 CS4280 Professor Hauschild
 *
 * Des...
 */

#include <stdio.h>
#include <stdlib.h>
#include "staticSem.h"

token tkStack[100]; // Stack to hold tokens
static int numVars = 0; // Number of variables
static int startScope = 0; // Start point of scope
static int tempVars = 0; // Number of temporary variables
static int t = 0;
static int numLoops = 0; // Number of loops
static bool isGlobal = true; // If variables are global
static bool isLoop = false;
static int globalVars = 0; // Number of global variables

// Function to initialize all stack locations with an empty string
void initStack(void) {
	for(int i = 0 ; i < 101 ; i++) { tkStack[i].tkName = ""; }

	return;
}

// Function to push tokens onto stack
void push(token stackTk) {
	// Catch error if stack is full or if variable has already been declared in same scope
	if(numVars == 100) {
		cout << "ERROR: Stack is full\n";
		exit(1);
	}
	
	for(int i = startScope ; i < numVars ; i++) {
		if(tkStack[i].tkName == stackTk.tkName) {
			cout << "ERROR: " << stackTk.tkName << " has already been declared in this scope at line "<< tkStack[i].lineNum  << "\n";
			exit(1);
		}
	}

	// If not error, push token onto stack and increment variable count
	tkStack[numVars] = stackTk;
	tkStack[numVars].scope = startScope;
	stackTk.scope = startScope;
	numVars++;
}

// Function to pop tokens off stack
void pop(int startScope, ofstream& outFile) {
	for(int i = 0 ; i < numVars ; i++) {
		//cout << "tkStack[" << i << "].tkName and scope = " << tkStack[i].tkName << " " << tkStack[i].scope << " compared to startScope of " << startScope << "\n";
		if(tkStack[i].scope == startScope) {
			numVars--;
			tempVars--;
			outFile << "POP\n";
			//cout << "debug pop: " << tkStack[i].tkName << "\n";
			tkStack[i].tkName = "";
		}
	}
}

// Function to search if variable already exists in current local scope -- if so, return position
int find(string str) {
	for(int i = 0 ; i < 100 ; i++) {
		if(tkStack[i].tkName == str && tkStack[i].scope == startScope) { return (numVars - 1 - i); } 
	}

	return -1;
}

// Function to search if variable already exists in current or parent's scope -- if so, return position 
int varExists(string str) {
	for(int i = 0 ; i < numVars ; i++) {
		//cout << "i: " << i << "\n";
		//cout << "DEBUG: " << tkStack[i].tkName << " compared to " << str << "\n";
		//cout << "Scope: " << tkStack[i].scope << "\n";
		if(tkStack[i].tkName == str && tkStack[i].scope <= startScope) { 
			return i; 
			//return (numVars - 1 - i);
		}
	}

	return -1;
}

void semanticCheck(node *node, int count, ofstream& outFile) {
	// Check if null
	if(node == NULL) { return; }
	
	// Process <program> ->  <vars> main <block>
	if(node->name == "<program>") {
		int vars = 0;
		
		// Process children of <program>	
		if(node->child1 != NULL) { semanticCheck(node->child1, vars, outFile); }
		if(node->child2 != NULL) { semanticCheck(node->child2, vars, outFile); }
		
		// Retain global variables
		int i = globalVars;
		token globalVarsStack[globalVars];
		while(i > 0) {
			globalVarsStack[i-1] = tkStack[globalVars-i];
			i--;
		}

		pop(startScope, outFile);
		startScope--;
		
		outFile << "STOP\n";
			
		// Print global variables
		i = globalVars;
		while(i > 0) {
			outFile << globalVarsStack[i-1].tkName << " 0\n";
			i--;
		}
		
		// Print local variables
		while(t > 0) {
			outFile << "T" << t-1 << " 0\n";
			t--;
		}

		return;
	} 
	// Process <vars> -> empty | data Identifier := Integer ; <vars>
	else if(node->name == "<vars>") {
		int exists;
		exists = find(node->nToken2.tkName);
		
		if(exists == -1 || exists > count) {
			if(isGlobal == true) {
				push(node->nToken2);
				outFile << "PUSH\n";
				outFile << "LOAD " << node->nToken4.tkName << "\n";
				outFile << "STORE " << node->nToken2.tkName << "\n";
				globalVars++;
				count++;
			} else {
				push(node->nToken2);
				outFile << "PUSH\n";
				outFile << "LOAD " << node->nToken4.tkName << "\n";
				outFile << "STORE T" << tempVars << "\n";
				exists = varExists(node->nToken2.tkName);
				outFile << "STACKW " << (numVars - 1 - exists) << "\n";
				node->nToken2.scope = startScope;
				t++;
				tempVars++; 
				count++;
			}
		} else if(exists < count) {
			cout << "ERROR: " << node->nToken2.tkName << " has already been declared in this scope on line " << node->nToken2.lineNum << "\n";
                        exit(1);
		}

		if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
		if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
                if(node->child3 != NULL) { semanticCheck(node->child3, count, outFile); }
		if(node->child4 != NULL) { semanticCheck(node->child4, count, outFile); }
	}
	// Process <block> -> begin <vars> <stats> end
	else if(node->name == "<block>") {
		int vars = 0;
		startScope++;
		isGlobal = false;

		if(node->child1 != NULL) { semanticCheck(node->child1, vars, outFile); }
                if(node->child2 != NULL) { semanticCheck(node->child2, vars, outFile); }
		
		pop(startScope, outFile);
		startScope--;	
	}
	// Process <expr> -> <N>-<expr> | <N>
	else if(node->name == "<expr>") {
		if(node->nToken.tkIdentifier == minusTk) {
			int localV = tempVars;
			tempVars++;
			t++;
			
			if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
			outFile << "STORE T" << localV << "\n";

			if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
			outFile << "SUB T" << localV << "\n";

		} else {
			if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
		} 
	}
	// Process <N> -> <A>/<N> | <A>*<N> | <A>
	else if(node->name == "<N>") {
		if(node->nToken.tkIdentifier == slashTk) {
			int localV = tempVars;
			tempVars++;
			t++;

			if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
			outFile << "STORE T" << localV << "\n";

			if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
			outFile << "DIV T" << localV << "\n";
		} else if(node->nToken.tkIdentifier == asteriskTk) {
			int localV = tempVars;
                        tempVars++;
			t++;

                        if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
                        outFile << "STORE T" << localV << "\n";

                        if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                        outFile << "MULT T" << localV << "\n";
		} else {
			if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
		}
	}
	// Process <A> -> <M>+<A> | <M>
	else if(node->name == "<A>") {
		if(node->nToken.tkIdentifier == plusTk) {
			int localV = tempVars;
                        tempVars++;
			t++;

                        if(node->child2 != NULL) {semanticCheck(node->child2, count, outFile); }
                        outFile << "STORE T" << localV << "\n";

                        if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                        outFile << "ADD T" << localV << "\n";
		} else {
			if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
		}
	}
	// Process <M> -> *<M> | <R>
	else if(node->name == "<M>") {
		if(node->nToken.tkIdentifier == asteriskTk) {
			if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
			outFile << "MULT -1\n";
		} else {
			if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
		}
	}
	// Process <R> -> ( <expr> ) | Identifier | Integer
	else if(node->name == "<R>") {
		if(node->nToken.tkIdentifier == open_parTk) {
			if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
		} else if(node->nToken.tkIdentifier == idTk) {
			int exists = varExists(node->nToken.tkName);
			if(exists == -1) {
				cout << "ERROR: " << node->nToken.tkName << " at line " << node->nToken.lineNum <<" was not declared in this scope\n";
				exit(1); 
			}
			
			if(tkStack[exists].scope == 0) {
				outFile << "LOAD " << tkStack[exists].tkName << "\n";
			} else {
				outFile << "STACKR " << (numVars - 1 - exists) << "\n";
			}
		} else if(node->nToken.tkIdentifier == numTk) {
			outFile << "LOAD " << node->nToken.tkName << "\n";
		}
	}
	// Process <stats> -> <stat> <mStats>
	else if(node->name == "<stats>") {
		if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
	}
	// Process <mStat> -> empty | <stat> <mStat>
	else if(node->name == "<mStat>") {
		if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
	}
	// Process <stat> -> <in>; | <out>; | <block> | <if>; | <loop>; | <assign>; | <goto>; | <label>;
	else if(node->name == "<stat>") {
		if(node->child1->name == "<in>") {
			if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
		} else if(node->child1->name == "<out>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                } else if(node->child1->name == "<block>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                } else if(node->child1->name == "<if>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                } else if(node->child1->name == "<loop>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                } else if(node->child1->name == "<assign>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                } else if(node->child1->name == "<goto>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                } else if(node->child1->name == "<label>") {
                        if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                }
	}
	// Process <in> -> getter Identifier
	else if(node->name == "<in>") {
		if(node->nToken.tkIdentifier == getterTk) {
			int exists = varExists(node->nToken2.tkName);
	
			if(exists == -1) {
                	        int localV = tempVars;
                                tempVars++;
                                t++;

                                outFile << "PUSH\n";
                                outFile << "READ T" << localV << "\n";
                                outFile << "LOAD T" << localV << "\n";
                                exists = varExists(node->nToken2.tkName);
                                outFile << "STACKW " << (numVars - 1 - exists) << "\n";
	                } else {
				if(tkStack[exists].scope == 0) {
	                	       	outFile << "READ " << node->nToken2.tkName << "\n";
        		      	} else if(tkStack[exists].scope > 0) {
                        		outFile << "READ T" << (numVars - 1 - exists) << "\n";
                        		outFile << "STACKW " << (numVars - 1 - exists) << "\n";
                		}
			}
		}
	}
	// Process <out> -> outter <expr>
	else if(node->name == "<out>") {
		if(node->nToken.tkIdentifier == outterTk) {
			if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
			
			int localV = tempVars;
			tempVars++;
			t++;
			
			outFile << "STORE T" << localV << "\n";
			outFile << "WRITE T" << localV << "\n";
		}
	}
	// Process <if> -> if [ <expr> <RO> <expr> ] then <stat>
	else if(node->name == "<if>") {
		int loop = ++numLoops;
		outFile << "loop" << loop << ": ";
		string RO = node->child2->nToken.tkName;
		if(node->child3 != NULL) { semanticCheck(node->child3, count, outFile); }
		int localV = tempVars;
		tempVars++;
		t++;

		outFile << "STORE T" << localV << "\n";
		if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
		
		if(RO == "=>") {
			outFile << "SUB T" << localV << "\n";
			if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
                } else if(RO == "=<") {
                        outFile << "SUB T" << localV << "\n";
			if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
                } else if(RO == "==") {
                        outFile << "SUB T" << localV << "\n";
                        if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
                } else if(RO[0] == '[') {
                        outFile << "SUB T" << localV << "\n";
                        if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
                } else if(RO == "%") {
                        outFile << "MULT T" << localV << "\n";
                        if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
                }

                if(node->child4 != NULL) { semanticCheck(node->child4, count, outFile); }
                outFile << "loop" << (loop + 1) << ": NOOP\n";
		numLoops++;
	}
	// Process <loop> -> loop [ <expr> <RO> <expr> ] <stat>
	else if(node->name == "<loop>") {
		int loop = ++numLoops;
                outFile << "loop" << loop << ": ";
		string RO = node->child2->nToken.tkName;
                if(node->child3 != NULL) { semanticCheck(node->child3, count, outFile); }
		int localV = tempVars;
		tempVars++;
		t++;

		outFile << "STORE T" << localV << "\n";
		if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }

		if(RO == "=>") {
			outFile << "SUB T" << localV << "\n";
			if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
		} else if(RO == "=<") {
			outFile << "SUB T" << localV << "\n";
			if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
		} else if(RO == "==") {
			outFile << "SUB T" << localV << "\n";
			if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
		} else if(RO[0] == '[') {
			outFile << "SUB T" << localV << "\n";
			if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
		} else if(RO == "%") {
			outFile << "MULT T" << localV << "\n";
			if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
		}

                if(node->child4 != NULL) { semanticCheck(node->child4, count, outFile); }
                outFile << "BR loop" << loop << "\n";
                outFile << "loop" << (loop + 1) << ": NOOP\n";
		numLoops++;
	}
	// Process <assign> -> assign Identifier := <expr>
	else if(node->name == "<assign>") {
		int exists = varExists(node->nToken2.tkName);

		if(exists == -1) {
                	cout << "ERROR: " << node->nToken2.tkName << " at line " << node->nToken2.lineNum << " was not declared in this scope\n";
                	exit(1);
                }

		if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }

		if(tkStack[exists].scope == 0) {
			outFile << "STORE " << node->nToken2.tkName << "\n";
		} else if(tkStack[exists].scope > 0) {
			outFile << "STORE T" << (numVars - 1 - exists) << "\n";
			outFile << "STACKW " << (numVars - 1 - exists) << "\n";
		}
	}
	// Process <RO> -> => | =< | == | [==] | %
	else if(node->name == "<RO>") {
                if(node->nToken.tkIdentifier == great_eqTk) {
                        outFile << "BRNEG loop" << (numLoops + 1) << "\n";
                } else if(node->nToken.tkIdentifier == less_eqTk) {
                        outFile << "BRPOS loop" << (numLoops + 1) << "\n";
                } else if(node->nToken.tkIdentifier == double_equalsTk) {
			outFile << "BRPOS loop" << (numLoops + 1) << "\n";
                        outFile << "BRNEG loop" << (numLoops + 1) << "\n";
                } else if(node->nToken.tkIdentifier == open_bracketTk) {
                        outFile << "BRZERO loop" << (numLoops + 1) << "\n";
                } else if(node->nToken.tkIdentifier == modulusTk) {
                        outFile << "BRPOS loop" << (numLoops + 1) << "\n";
                }
        }
	// Process <label> -> void Identifier
	else if(node->name == "<label>") {
		int exists = find(node->nToken2.tkName);

                if(exists == -1 || exists > count) {
                        push(node->nToken2);
			outFile << node->nToken2.tkName << ": NOOP\n";
                        count++;
                } else if(exists < count) {
                        cout << "ERROR: " << node->nToken2.tkName << " has already been declared in this scope on line " << node->nToken2.lineNum << "\n";
                        exit(1);
                }
			
                if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
	}
	// Process <goto> -> proc Identifier
	else if(node->name == "<goto>") {
		int exists = varExists(node->nToken2.tkName);
                if(exists == -1) {
			cout << "ERROR: " << node->nToken2.tkName << " at line " << node->nToken2.lineNum << " was not declared in this scope\n";
                        exit(1);
                }
		
		outFile << "BR " << node->nToken2.tkName << "\n";

                if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
	}
	else {
		if(node->child1 != NULL) { semanticCheck(node->child1, count, outFile); }
                if(node->child2 != NULL) { semanticCheck(node->child2, count, outFile); }
                if(node->child3 != NULL) { semanticCheck(node->child3, count, outFile); }
                if(node->child4 != NULL) { semanticCheck(node->child4, count, outFile); }
	}
}
