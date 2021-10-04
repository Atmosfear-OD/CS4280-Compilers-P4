/*
 * parser.cpp by Pascal Odijk 4/29/2021
 * P4 CS4280 Professor Hauschild
 *
 * This file contains the parser to parse through the tokens passed to it by the scanner. The parser utilizes the BNF as specified to ensure the tokens are used correctly.
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "parser.h"

int level = 0;
int lineNum = 1;
token tk;

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

// Create new node
node* getNode(string n) {
	node *newNode = new node;
	newNode->name = n;
	newNode->level = level;

	newNode->child1 = NULL;
	newNode->child2 = NULL;
	newNode->child3 = NULL;
	newNode->child4 = NULL;
	
	return newNode;
}

// <program> -> <vars> main <block>
node* program(ifstream& file) {
	node *newNode = getNode("<program>");
	level++;
	newNode->child1 = vars(file);
	
	if(tk.tkIdentifier == mainTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		newNode->child2 = block(file);

		return newNode;
	} else {
		cout << "PARSER ERROR: Invalid token -- expected Main Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
		exit(1);
	}
}

// <vars> -> empty | data Identifier := Integer ; <vars>
node* vars(ifstream& file) {
	//level++;
	node *newNode = getNode("<vars>");

	if(tk.tkIdentifier == dataTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		
		if(tk.tkIdentifier == idTk) {
			newNode->nToken2 = tk;
			tk = scanner(file, lineNum);

			if(tk.tkIdentifier == colon_equalsTk) {
				newNode->nToken3 = tk;
				tk = scanner(file, lineNum);
			
				if(tk.tkIdentifier == numTk) {
					newNode->nToken4 = tk;
					tk = scanner(file, lineNum);
					
					if(tk.tkIdentifier == semicolonTk) {
						newNode->nToken5 = tk;
						tk = scanner(file, lineNum);
						level++;
						newNode->child1 = vars(file);
						return newNode;
					} else {
						cout << "PARSER ERROR: Invalid token -- expected Semicolon but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
						exit(1);
					}
				} else {
					cout << "PARSER ERROR: Invalid token -- expected Integer but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
					exit(1);
				}
			} else {
				cout << "PARSER ERROR: Invalid token -- expected Colon Equals but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
				exit(1);
			}
		} else {
			cout << "PARSER ERROR: Invalid token -- expected Identifier but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
			exit(1);
		}
	} else {
		return NULL;
	}
}

// <block> -> begin <vars> <stats> end
node* block(ifstream& file) {
	node *newNode = getNode("<block>");

	if(tk.tkIdentifier == beginTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		level++;
		newNode->child1 = vars(file);
		newNode->child2 = stats(file);
		//tk = scanner(file, lineNum);

		if(tk.tkIdentifier == endTk) {
			newNode->nToken2 = tk;
			tk = scanner(file, lineNum);
			return newNode;
		} else {
			cout << "PARSER ERROR: Invalid token -- expected End Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
			exit(1);
		}
	} else {
		cout << "PARSER ERROR: Invalid token -- expected Begin Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
		exit(1);
	}
}

// <expr> -> <N>-<expr> | <N>
node* expr(ifstream& file) {
	level++;
	node *newNode = getNode("<expr>");

	newNode->child1 = N(file);

	if(tk.tkIdentifier == minusTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		newNode->child2 = expr(file);

		return newNode;
	} else {
		return newNode;
	}
}

// <N> -> <A>/<N> | <A>*<N> | <A>
node* N(ifstream& file) {
	level++;
	node *newNode = getNode("<N>");

	newNode->child1 = A(file);

	if(tk.tkIdentifier == slashTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		newNode->child2 = N(file);

		return newNode;
	} else if(tk.tkIdentifier == asteriskTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		newNode->child2 = N(file);

		return newNode;
	} else {
		return newNode;
	}
}

// <A> -> <M>+<A> | <M>
node* A(ifstream& file) {
	level++;
	node *newNode = getNode("<A>");

	newNode->child1 = M(file);

	if(tk.tkIdentifier == plusTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		newNode->child2 = A(file);

		return newNode;
	} else {
		return newNode;
	}
}

// <M> -> *<M> | <R>
node* M(ifstream& file) {
	level++;
	node *newNode = getNode("<M>");

	if(tk.tkIdentifier == asteriskTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		newNode->child1 = M(file);

		return newNode;
	} else {
		newNode->child1 = R(file);
		return newNode;
	}
}

// <R> -> ( <expr> ) | Identifier | Integer
node* R(ifstream& file) {
	level++;
	node *newNode = getNode("<R>");

	if(tk.tkIdentifier == open_parTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		newNode->child1 = expr(file);

		if(tk.tkIdentifier == close_parTk) {
			newNode->nToken2 = tk;
			tk = scanner(file, lineNum);
			return newNode;
		} else {
			cout << "PARSER ERROR: Invalid token -- expected Close Parenthesis but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
			exit(1);
		}
	} else if(tk.tkIdentifier == idTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);

		return newNode;
	} else if(tk.tkIdentifier == numTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);

		return newNode;
	} else {
		cout << "PARSER ERROR: Invalid token -- expected Open Parenthesis but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
		exit(1);
	}
}

// <stats> -> <stat> <mStat>
node* stats(ifstream& file) {
	level++;
	node *newNode = getNode("<stats>");
	
	newNode->child1 = stat(file);
	newNode->child2 = mStat(file);

	return newNode;
}

// <mStat> -> empty | <stat> <mStat>
node* mStat(ifstream& file) {
	if(tk.tkIdentifier == getterTk || tk.tkIdentifier == outterTk || tk.tkIdentifier == beginTk || tk.tkIdentifier == ifTk || tk.tkIdentifier == loopTk ||
		tk.tkIdentifier == assignTk || tk.tkIdentifier == procTk || tk.tkIdentifier == voidTk) {
		level++;
		node *newNode = getNode("<mStat>");
		
		newNode->child1 = stat(file);
		newNode->child2 = mStat(file);

		return newNode;
	} else {
		return NULL;
	}
}

// <stat> -> <in>; | <out>; | <block> | <if>; | <loop>; | <assign>; | <goto>; | <label>;
node* stat(ifstream& file) {
	level++;
	node *newNode = getNode("<stat>");
	
	if(tk.tkIdentifier == getterTk) {
		newNode->child1 = in(file);
		//tk = scanner(file, lineNum);
		if(tk.tkIdentifier == semicolonTk) {
			newNode->nToken = tk;
			tk = scanner(file, lineNum);

			return newNode;
		} else {
			cout << "PARSER ERROR: Invalid token -- expected Semicolon but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
			exit(1);
		}
	} else if(tk.tkIdentifier == outterTk) {
		newNode->child1 = out(file);
		//tk = scanner(file, lineNum);

                if(tk.tkIdentifier == semicolonTk) {
                        newNode->nToken = tk;
                        tk = scanner(file, lineNum);

                        return newNode;
                } else {
                        cout << "PARSER ERROR: Invalid token -- expected Semicolon but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                        exit(1);
                }
	} else if(tk.tkIdentifier == beginTk) {
		//level++;
		newNode->child1 = block(file);
		return newNode;
	} else if(tk.tkIdentifier == ifTk) {
		newNode->child1 = if_F(file);
		//tk = scanner(file, lineNum);

                if(tk.tkIdentifier == semicolonTk) {
                        newNode->nToken = tk;
                        tk = scanner(file, lineNum);

                        return newNode;
                } else {
                        cout << "PARSER ERROR: Invalid token -- expected Semicolon but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                        exit(1);
                }
	} else if(tk.tkIdentifier == loopTk) {
		newNode->child1 = loop(file);
		//tk = scanner(file, lineNum);

                if(tk.tkIdentifier == semicolonTk) {
                        newNode->nToken = tk;
                        tk = scanner(file, lineNum);

                        return newNode;
                } else {
                        cout << "PARSER ERROR: Invalid token -- expected Semicolon but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                        exit(1);
                }
	} else if(tk.tkIdentifier == assignTk) {
		newNode->child1 = assign(file);
		//tk = scanner(file, lineNum);

                if(tk.tkIdentifier == semicolonTk) {
                        newNode->nToken = tk;
                        tk = scanner(file, lineNum);

                        return newNode;
                } else {
                        cout << "PARSER ERROR: Invalid token -- expected Semicolon but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                        exit(1);
                }
	} else if(tk.tkIdentifier == procTk) {
		newNode->child1 = goto_F(file);
		//tk = scanner(file, lineNum);

                if(tk.tkIdentifier == semicolonTk) {
                        newNode->nToken = tk;
                        tk = scanner(file, lineNum);

                        return newNode;
                } else {
                        cout << "PARSER ERROR: Invalid token -- expected Semicolon but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                        exit(1);
                }
	} else if(tk.tkIdentifier == voidTk) {
		newNode->child1 = label(file);
		//tk = scanner(file, lineNum);

                if(tk.tkIdentifier == semicolonTk) {
                        newNode->nToken = tk;
                        tk = scanner(file, lineNum);

                        return newNode;
                } else {
                        cout << "PARSER ERROR: Invalid token -- expected Semicolon but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                        exit(1);
                }
	} else {
		cout << "PARSER ERROR: Expected Getter, Outter, Begin, If, Loop, Assign, Proc, or Void Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
		exit(1);
	}
}

// <in> -> getter Identifier
node* in(ifstream& file) {
	level++;
	node *newNode = getNode("<in>");
	
	if(tk.tkIdentifier == getterTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);

		if(tk.tkIdentifier == idTk) {
			newNode->nToken2 = tk;
			tk = scanner(file, lineNum);

			return newNode;
		} else {
			cout << "PARSER ERROR: Invalid token -- expected Identifier but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
			exit(1);
		}
	} else {
		cout << "PARSER ERROR: Invalid token -- expected Getter Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
		exit(1);
	}
}

// <out> -> outter <expr>
node* out(ifstream& file) {
	level++;
        node *newNode = getNode("<out>");

        if(tk.tkIdentifier == outterTk) {
		newNode->nToken = tk;
       		tk = scanner(file, lineNum);
		newNode->child1 = expr(file);
	
		return newNode;
        } else {
                cout << "PARSER ERROR: Invalid token -- expected Outter Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                exit(1);
        }
}

// <if> -> if [ <expr> <RO> <expr> ] then <stat>
node* if_F(ifstream& file) {
	if(tk.tkIdentifier == ifTk) {
		level++;
		node *newNode = getNode("<if>");
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		
		if(tk.tkIdentifier == open_bracketTk) {
			newNode->nToken2 = tk;
			tk = scanner(file, lineNum);
			newNode->child1 = expr(file);
	                newNode->child2 = RO(file);
        	        newNode->child3 = expr(file);

			if(tk.tkIdentifier == close_bracketTk) {
				newNode->nToken3 = tk;
				tk = scanner(file, lineNum);
				
				if(tk.tkIdentifier == thenTk) {
					newNode->nToken4 = tk;
					tk = scanner(file, lineNum);
					newNode->child4 = stat(file);

					return newNode;
				} else {
					cout << "PARSER ERROR: Invalid token -- expected Then Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                        		exit(1);
				}	
			} else {
				cout << "PARSER ERROR: Invalid token -- expected Close Bracket but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                        	exit(1);
			}
		} else {
			cout << "PARSER ERROR: Invalid token -- expected Open Bracket but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
			exit(1);
		}
	} else {
		cout << "PARSER ERROR: Invalid token -- expected If Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
		exit(1); 
	}
}

// <loop> -> loop [ <expr> <RO> <expr> ] <stat>
node* loop(ifstream& file) {
	if(tk.tkIdentifier == loopTk) {
		level++;
		node *newNode = getNode("<loop>");
		newNode->nToken = tk;
		tk = scanner(file, lineNum);

		if(tk.tkIdentifier == open_bracketTk) {
			newNode->nToken2 = tk;
			tk = scanner(file, lineNum);
			newNode->child1 = expr(file);
			newNode->child2 = RO(file);
                        newNode->child3 = expr(file);

			if(tk.tkIdentifier == close_bracketTk) {
				newNode->nToken3 = tk;
                                tk = scanner(file, lineNum);
                                newNode->child4 = stat(file);

                                return newNode;
                        } else {
                                cout << "PARSER ERROR: Invalid token -- expected Close Bracket but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                                exit(1);
                        }
		
		} else {
                        cout << "PARSER ERROR: Invalid token -- expected Open Bracket but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                        exit(1);
                }
	} else {
		cout << "PARSER ERROR: Invalid token -- expected Loop Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                exit(1);
	}
}

// <assign> -> assign Identifier := <expr>
node* assign(ifstream& file) {
	if(tk.tkIdentifier == assignTk) {
		level++;
		node *newNode = getNode("<assign>");
		newNode->nToken = tk;
		tk = scanner(file, lineNum);

		if(tk.tkIdentifier == idTk) {
			newNode->nToken2 = tk;
			tk = scanner(file, lineNum);

			if(tk.tkIdentifier == colon_equalsTk) {
				newNode->nToken3 = tk;
				tk = scanner(file, lineNum);
				newNode->child1 = expr(file);

				return newNode;
			} else {
				cout << "PARSER ERROR: Invalid token -- expected Colon Equals but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                		exit(1);
			}
		} else {
			cout << "PARSER ERROR: Invalid token -- expected Identifier but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                	exit(1);	
		}
	} else {
		cout << "PARSER ERROR: Invalid token -- expected Assign Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                exit(1);
	}
}

// <RO> -> => | =< | == | [ == ] (three tokens) | %
node* RO(ifstream& file) {
	level++;
	node *newNode = getNode("<RO>");

	if(tk.tkIdentifier == great_eqTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		return newNode;
	} else if(tk.tkIdentifier == less_eqTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		return newNode;
	} else if(tk.tkIdentifier == double_equalsTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		return newNode;
	} else if(tk.tkIdentifier == open_bracketTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		if(tk.tkIdentifier == double_equalsTk) {
			newNode->nToken2 = tk;
			tk = scanner(file, lineNum);
			if(tk.tkIdentifier == close_bracketTk) {
				newNode->nToken3 = tk;
				tk = scanner(file, lineNum);
				return newNode;
			} else {
				cout << "PARSER ERROR: Invalid operator at line " << level << " -- expected [ == ]\n";
        	                exit(1);
			}
		} else {
			cout << "PARSER ERROR: Invalid operator at line " << level << " -- expected [ == ]\n";
			exit(1); 
		}
	} else if(tk.tkIdentifier == modulusTk) {
		newNode->nToken = tk;
		tk = scanner(file, lineNum);
		return newNode;
	} else {
		cout << "PARSER ERROR: Invalid token -- expected =< , =< , == , [ == ] , or % but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
		exit(1);
	}
}

// <label> -> void Identifier
node* label(ifstream& file) {
	if(tk.tkIdentifier == voidTk) {
		level++;
		node *newNode = getNode("<label>");
		newNode->nToken = tk;
		tk = scanner(file, lineNum);

		if(tk.tkIdentifier == idTk) {
			newNode->nToken2 = tk;
			tk = scanner(file, lineNum);

			return newNode;
		} else {
			cout << "PARSER ERROR: Invalid token -- expected Identifier but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                	exit(1);
		}
	} else {
		cout << "PARSER ERROR: Invalid token -- expected Void Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                exit(1);
	}
}

// <goto> -> proc Identifier
node* goto_F(ifstream& file) {
	if(tk.tkIdentifier == procTk) {
                level++;
                node *newNode = getNode("<goto>");
		newNode->nToken = tk;
                tk = scanner(file, lineNum);

                if(tk.tkIdentifier == idTk) {
                        newNode->nToken2 = tk;
                        tk = scanner(file, lineNum);

                        return newNode;
                } else {
                        cout << "PARSER ERROR: Invalid token -- expected Identifier but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                        exit(1);
                }
        } else {
                cout << "PARSER ERROR: Invalid token -- expected Proc Keyword but recieved " << tokenNames[tk.tkIdentifier] << " at line " << level << "\n";
                exit(1);
        }	
} 

// Parser function
node* parser(ifstream& ifile){
	node* nd;

        while(ifile) {
                tk = scanner(ifile, lineNum);
		nd = program(ifile); 
        }

        return nd;
}

