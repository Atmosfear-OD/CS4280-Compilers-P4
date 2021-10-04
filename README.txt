P4: Storage=Local
------------------------------------

To compile the project:
1. run 'make compfs' 
2. the program can be invoked three different ways: file specification (no extension), file redirection (with extension), and keyboard input
	file: ./compfs fileName
	redirection: ./compfs < fileName.ext
	keyboard: ./compfs 	// User then types input and presses ctrl+d to simulate EOF

Note: More than one argument will output an error message and instructions to properly run program
The asm produced by staticSem.cpp will be written to an output file name either fileName.asm if a file is used to invoke or kb.asm if either keyboard input or file redirection is used to invoke.
