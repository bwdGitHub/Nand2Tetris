// VMTranslator.cpp : Defines the entry point for the console application.
// Implement a VM translator according to the nand2tetris specification.

#include "stdafx.h"
#include <iostream>
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <string>
#include <fstream>
#include <bitset>
#include "Parser.h"
#include "CodeWriter.h"

int main(int argc, char* argv[])
{
	// check number of arguments
	if (argc != 2)
	{
		std::cerr << "Wrong number of arguments - please provide only a file name and extension." << std::endl;
		exit(1);
	}

	// get current directory

	char* currentDirectoryBuffer;
	std::string currentDirectory;

	if ((currentDirectoryBuffer = _getcwd(NULL, 0)) == NULL)
	{
		std::cerr << "Error getting the current working directory." << std::endl;
		exit(2);
	}
	else
	{
		currentDirectory = currentDirectoryBuffer; // parse char* into string.
		free(currentDirectoryBuffer);
	}

	// get address of file

	std::string filename = argv[1];

	int dot = filename.find('.');

	if (dot == std::string::npos || filename.substr(dot) != ".vm")
	{
		std::cerr << "File name didn't contain the appropriate file extension." << std::endl;
		exit(3);
	}

	std::string fname_no_ext = filename.substr(0, dot);

	std::string address = currentDirectory + "\\" + argv[1];

	// instantiate parser and codewriter

	Parser parser(address);
	CodeWriter codewriter(currentDirectory, fname_no_ext);

	// go through lines of vm file

	while (parser.nextCommand())
	{
		// get command type for the line
		command_type ctype = parser.commandType();
		
		// returns dealt with separately
		if (ctype != command_type::C_RETURN)
		{
			std::string arg_one = parser.argOne();
			// deal with arithmetic
			if (ctype == command_type::C_ARITHMETIC)
			{
				codewriter.writeArithmetic(arg_one);
			}
			if (ctype == command_type::C_PUSH || ctype == command_type::C_POP)
			{
				int arg_two = parser.argTwo();
				codewriter.writePushPop(ctype, arg_one, arg_two);
			}
		}
	}
	
	

    return 0;
}

