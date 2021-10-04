/*
 * main.cpp by Pascal Odijk 4/29/2021
 * P4 CS4280 Professor Hauschild
 *
 * This file contains the main function for P4. The project starts by taking input in three different ways: file specification, redirection, or through keyboard input followed by ctrl+d (simulated EOF). 
 * If the user starts the program with ./compfs, this will trigger the keyboard input form, ./compfs < fileName.ext will trigger redirection, and ./compfs fileName will trigger file input. If more than one 
 * argument is encountered, the program will output an error message. A parse tree will then be built with the file input through the parser function and sent to semCheck to check the semantics. Finally, 
 * the asm produced by semCheck will be written to an output file named filename.asm if a file is provided or kb.asm if either keyboard input or file redirection.
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include "parser.h"
#include "staticSem.h"

using namespace std;

int main(int argc, char *argv[]) {
	string fileName;
	string outName;
	ifstream ifile;
	ofstream ofile;
	
	if(argc == 1){ // Handles program initialization of ./frontEnd (keyboard) or ./frontEnd < fileName.ext (redirection)
		try {
			outName = "kb.asm";
			string userInput;
			ofstream tempFile;
	
			fileName = "temp.sp2021";
	
			tempFile.open(fileName.c_str());

			while(getline(cin, userInput)) {
				tempFile << userInput << "\n";
			}

			tempFile.close();

		} catch(const ifstream::failure& e){
			cout << "Error file not found\n";
			return 1;
		}

	} else if(argc == 2){ // Handles program initialization of ./frontEnd fileName (read from file)	
		fileName = argv[1];
		outName = argv[1];
		outName.append(".asm");
		fileName.append(".sp2021");

	} else if(argc > 2){ // Error if more than one argument is entered
		cout << "ERROR: Too many arguments\n";
		cout << "USAGE: ./scanner [keyboard input], or ./scanner fileName, or ./scanner < fileName.ext\n";
		return 1;
	}
	
	// Open input file
	try {
		ifile.open(fileName.c_str());
	} catch(const ifstream::failure& e) {
		cout << "ERROR: File not found\n";
		return 1;
	}

	// Open output file
	try {
		ofile.open(outName.c_str());
	} catch(const ofstream::failure& e) {
		cout << "ERROR: Could not open file\n";
		return 1;
	}

	// Build tree
	node *parseTree = parser(ifile);

	// Check semantics
	semanticCheck(parseTree, 0, ofile);

	// Close input and output files
	cout << "Compilation complete -- now writing to " << outName << "\n";
	ifile.close();
	ofile.close();
		
	return 0;
}
