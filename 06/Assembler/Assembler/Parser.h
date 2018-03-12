// Parser.h
// Header for the parser class. Takes a file name and opens a filestream for that file.
// Multiple functions for parsing the filestream.

#pragma once
#include "stdafx.h"
#include <fstream>
#include <string>

class Parser
{
// currentCommand and stream  kept private.
// Prevents them being manipulated by anything other than the below class methods.

private:
	std::string currentCommand;
	std::ifstream stream;

public:
	// Constructor, opens the inFile.
	Parser(std::string address);
	~Parser();

	// Returns if there are more commands.
	//bool hasMoreCommands();

	// Reads the next command. Only if hasMoreCommands is true.
	//void advance();

	bool hasMoreCommandsAndAdvance();

	// Returns A_COMMAND for A commands, C for C, and L for pseudos, (Xxx).
	// Note, maybe use a typedef or something here.
	std::string commandType();

	// Return the symbol of Xxx if @Xxx or (Xxx) called, 
	// and command type is A or L.
	std::string symbol(); 

	// Returns the dest mnemonic in the current C-command, only
	// if the command is C.
	std::string dest();

	// Returns the comp mnemonic if it is C-command.
	std::string comp();

	// Returns jump mnemonic if it is C-command.
	std::string jump();

};