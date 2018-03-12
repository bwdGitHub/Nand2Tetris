#include "stdafx.h"
#include "Parser.h"
#include <string>
#include <algorithm>

enum class PARSER_ERROR { OPEN_FILE_ERROR = 1, COMMAND_SYNTAX = 2, RETURN_ARG_ERROR = 3, NO_SECOND_ARG = 4};

Parser::Parser(string address)
{
	command = "";
	stream >> std::skipws;
	stream.open(address);
	if (!stream)
	{
		std::cerr << "Unable to open file"<<std::endl;
		exit(static_cast<int>(PARSER_ERROR::OPEN_FILE_ERROR));
	}
}

Parser::~Parser()
{
	stream.close();
}

bool Parser::nextCommand()
{
	string line;

	stream >> std::ws;

	command = "";
	while (command == "" && getline(stream,line))
	{
		int positionOfSlash = line.find('/');
		if (positionOfSlash != string::npos)
		{
			line = line.substr(0, positionOfSlash);
		}
		command = line;
	}
	if (command != "")
	{
		return true;
	}
	return false;
}

command_type Parser::commandType()
{
	// arithmetic commands
	int add = command.find("add");
	int sub = command.find("sub");
	int neg = command.find("neg");
	int eq = command.find("eq");
	int gt = command.find("gt");
	int lt = command.find("lt");
	int and = command.find("and");
	int or = command.find("or");
	int not = command.find("not");

	if (add > -1 || sub > -1 || neg > -1 || eq > -1 || gt > -1 || lt > -1 || and > -1 || or > -1 || not> -1)
	{
		return command_type::C_ARITHMETIC;
	}

	if (command.find("pop") != string::npos)
	{
		return command_type::C_POP;
	}
	if (command.find("push") != string::npos)
	{
		return command_type::C_PUSH;
	}
	if (command.find("label") != string::npos) return command_type::C_LABEL;
	if (command.find("goto") != string::npos) return command_type::C_GOTO;
	if (command.find("if-goto") != string::npos) return command_type::C_IF;
	if (command.find("function") != string::npos) return command_type::C_FUNCTION;
	if (command.find("call") != string::npos) return command_type::C_CALL;
	if (command.find("return") != string::npos) return command_type::C_RETURN;
	cerr << "Syntax Error - Unrecognised Command Type" << endl;
	exit(static_cast<int>(PARSER_ERROR::COMMAND_SYNTAX));
}

string Parser::argOne()
{
	if (Parser::commandType() == command_type::C_RETURN)
	{
		cerr << "Cannot call Parser::argOne on C_Return commands." << endl;
		exit(static_cast<int>(PARSER_ERROR::RETURN_ARG_ERROR));
	}
	else
	{
		int whitespaceOne = command.find(" ");
		if (whitespaceOne == -1)
		{
			return command;
		}
		else
		{
			string arguments = command.substr(whitespaceOne+1);
			int whitespaceTwo = arguments.find(" ");
			return arguments.substr(0, whitespaceTwo);
		}
	}
}

int Parser::argTwo()
{
	command_type type = Parser::commandType();
	if (type == command_type::C_PUSH || type == command_type::C_POP || type == command_type::C_FUNCTION || type == command_type::C_CALL)
	{
		int whitespaceOne = command.find(" ");
		string arguments = command.substr(whitespaceOne+1);
		int whitespaceTwo = arguments.find(" ");
		return stoi(arguments.substr(whitespaceTwo + 1));
	}
	else
	{
		cerr << "Parser::argTwo error - Command does not have second argument." << endl;
		exit(static_cast<int>(PARSER_ERROR::NO_SECOND_ARG));
	}
}
