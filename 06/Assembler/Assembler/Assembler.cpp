// Assembler.cpp : Defines the entry point for the console application.
// Parses assembly language into machine code according to the nand2tetris specification.

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include "Parser.h"
#include "Code.h"
#include "SymbolTable.h"


// The main function should only return 0 when the program runs properly. 
// I've used an enum class here to keep track of errors, and return non-zero ints when they occur.
enum class ERROR {FILE_EXTENSION_ERROR = 1, COMMAND_LINE_ARG_ERROR=2, GETCWD_ERROR=3, OPEN_FILE_ERROR=4}; 

int main(int argc, char* argv[])
{
	// C-code to get the current working directory. See the microsoft documentation for _getcwd.
	char* currentDirectoryBuffer;
	std::string currentDirectory;
  
	if ((currentDirectoryBuffer = _getcwd(NULL, 0)) == NULL)
	{
		std::cerr << "Error getting the current working directory." << std::endl;
		return static_cast<int>(ERROR::GETCWD_ERROR);
	}
	else
	{
		currentDirectory = currentDirectoryBuffer; // parse char* into string.
		free(currentDirectoryBuffer);
	}

	// Check we have required number of arguments.
	if (argc == 2)
	{
		// Get address from command line.
		std::string address = argv[1];

		// Get output file name
		int dot = address.find('.');
		if (dot == std::string::npos || address.substr(dot)!=".asm")
		{
			std::cerr << "File name didn't contain the appropriate file extension." << std::endl;
			return static_cast<int>(ERROR::FILE_EXTENSION_ERROR); // Error codes, see the enum. Essentially, return non-zero when there's an error.
		}
		// Get filename, open a filestream to write machine code to.
		std::string filename = address.substr(0, dot);
		std::ofstream output;

		output.open(currentDirectory + "\\" +  filename + ".hack", std::ios_base::trunc); // trunc will clear the file, and open it file writing from the beginning.
		if (output.is_open())
		{
			Parser parser(currentDirectory + "\\" + address);
			SymbolTable table;
			int rom = 0;
			while (parser.hasMoreCommandsAndAdvance())
			{
			
				if (parser.commandType() == "A_COMMAND" || parser.commandType() == "C_COMMAND")
				{
					rom++;
				}
				if (parser.commandType() == "L_COMMAND")
				{
					table.addEntry(parser.symbol(), rom);
				}

			}
			
			Parser parserTwo(address);
			int ram = 16;
			while (parserTwo.hasMoreCommandsAndAdvance())
			{
				
				if (parserTwo.commandType() == "C_COMMAND")
				{
					std::string comp = Code::comp(parserTwo.comp());
					std::string dest = Code::dest(parserTwo.dest());
					std::string jump = Code::jump(parserTwo.jump());

					output << "111" << comp << dest << jump << std::endl;
				}

				else if (parserTwo.commandType() == "A_COMMAND")
				{
					std::string sym = parserTwo.symbol();
					bool isNum = (sym.find_first_not_of("0123456789") == std::string::npos);
					if (isNum)
					{
						std::string binary_symbol = std::bitset<16>(std::stoi(sym)).to_string();
						output << binary_symbol << std::endl;
					}
					else
					{
						if (table.contains(sym))
						{
							int address = table.GetAddress(sym);
							std::string binary_symbol = std::bitset<16>(address).to_string();
							output << binary_symbol << std::endl;
						}
						else
						{
							table.addEntry(sym, ram);
							std::string binary_symbol = std::bitset<16>(ram).to_string();
							output << binary_symbol << std::endl;
							ram++;
						}
					}
				}
			}

			output.close();
			return 0;
		}

		else
		{
			std::cerr << "Error opening the hack file for writing." << std::endl;
			return static_cast<int>(ERROR::OPEN_FILE_ERROR);
		}
	}
	else
	{
		std::cerr << "Wrong number of command line arguments." << std::endl;
		return static_cast<int>(ERROR::COMMAND_LINE_ARG_ERROR);
	}
}

