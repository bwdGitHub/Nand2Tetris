// Parser.cpp
// Implementation of the parser class.

#include "stdafx.h"
#include "Parser.h"
#include <iostream>
#include <algorithm>

// Error reporting with enum class. 
// Not sure if instead there should be a global error identifier across all project classes.
// e.g. This returns 1 when the asm file can't be opened, but assembler returns 1 when there is no file extension.
enum class PARSER_ERROR {OPEN_FILE_ERROR=1, EMPTY_COMMAND_ERROR=2, UNKNOWN_COMMAND_TYPE=3, SYMBOL_ERROR=4, DEST_ERROR = 5, COMP_ERROR=6, JUMP_ERROR=7};

Parser::Parser(std::string address)
{
	currentCommand = ""; //initialise currentCommand string to empty.

	// tell stream to skip whitespace
	stream >> std::skipws;

	// try to open the file at the address
	stream.open(address);

	if (!stream)
	{
		std::cerr << "Unable to open file";
		exit(static_cast<int>(PARSER_ERROR::OPEN_FILE_ERROR)); 
	}
}

Parser::~Parser()
{
	stream.close();
}

bool Parser::hasMoreCommandsAndAdvance()
{
	// get a temporary line
	std::string line;
	// try to read a line
	stream >> std::ws;
	if (std::getline(stream, line))
	{
		
		// remove comments.
		if (line.find('/') != std::string::npos)
		{
			int pos_of_slash = line.find('/');
			line = line.substr(0, pos_of_slash);
		}
		// remove remaining whitespace
		std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
		line.erase(end_pos, line.end());
		currentCommand = line;
		return true;
	}
	else
	{
		return false;
	}
}


// Note we could use an enum class to replace the A_COMMAND etc. strings with ints.

std::string Parser::commandType()
{
	if (currentCommand.length() == 0)
	{
		std::cerr << "Parser::commandType called when currentCommand empty." << std::endl;
		exit(static_cast<int>(PARSER_ERROR::EMPTY_COMMAND_ERROR));
	}
	// extract first character
	char firstChar = currentCommand[0];
	if (firstChar == '/' || currentCommand=="")
	{
		return "IGNORE";
	}
	else if (firstChar == '@')
	{
		return "A_COMMAND";
	}
	else if (firstChar == '(')
	{
		return "L_COMMAND";
	}
	else
	{
		return "C_COMMAND";
	}
}

std::string Parser::symbol()
{
	std::string type = Parser::commandType();
	if (type == "C_COMMAND" || type=="IGNORE")
	{
		std::cerr << "Parser::symbol called when currentCommand is not A or L.";
		exit(static_cast<int>(PARSER_ERROR::SYMBOL_ERROR));
	}
	else if (type == "A_COMMAND")
	{
		return currentCommand.substr(1);
	}

	else if (type=="L_COMMAND")
	{
		int size = currentCommand.size();
		return currentCommand.substr(1, size - 2);
	}

	else
	{
		std::cerr << "Parser::commandType() returned an unknown type." << std::endl;
		exit(static_cast<int>(PARSER_ERROR::UNKNOWN_COMMAND_TYPE));
	}
}

std::string Parser::dest()
{
	std::string type = Parser::commandType();
	if (type != "C_COMMAND")
	{
		std::cerr << "Parser::dest called when current command is not a C command." << std::endl;
		exit(static_cast<int>(PARSER_ERROR::DEST_ERROR));
	}
	else
	{
		int positionOfEquals = currentCommand.find("=");
		if (positionOfEquals == -1)
		{
			return "";
		}
		else
		{
			return currentCommand.substr(0, positionOfEquals);
		}
	}
	
}

std::string Parser::comp()
{
	std::string type = Parser::commandType();
	if (type != "C_COMMAND")
	{
		std::cerr << "Parser::comp called when current command is not a C command."<<std::endl;
		exit(static_cast<int>(PARSER_ERROR::COMP_ERROR));
	}
	else
	{
		int positionOfEquals = currentCommand.find("=");
		int positionOfSemicolon = currentCommand.find(";");
		if (positionOfEquals == -1)
		{
			return currentCommand.substr(0, positionOfSemicolon);
		}
		if (positionOfSemicolon = -1)
		{
			return currentCommand.substr(positionOfEquals + 1);
		}
		else
		{
			return currentCommand.substr(positionOfEquals + 1, positionOfSemicolon);
		}
	}
}

std::string Parser::jump()
{
	std::string type = Parser::commandType();
	if (type != "C_COMMAND")
	{
		std::cerr << "Parser::jump called when currentCommand is not C command.";
		exit(static_cast<int>(PARSER_ERROR::JUMP_ERROR));
	}
	else
	{
		int positionOfSemicolon = currentCommand.find(';');
		if (positionOfSemicolon == -1)
		{
			return "";
		}
		else
		{
			return currentCommand.substr(positionOfSemicolon+1);
		}
	}
}
