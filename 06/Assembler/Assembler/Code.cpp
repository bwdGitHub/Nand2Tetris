// Code.cpp
// Implementation of the Code class.

#include "stdafx.h"
#include <iostream>
#include "Code.h"
#include <string>

enum class CODE_ERROR { COMP_ERROR = 1 , JUMP_ERROR=2};

std::string Code::dest(std::string destInput)
{
	// Parse dest input, read the boolean into an int (representing binary).
	int hasM = (destInput.find('M') != std::string::npos);
	int hasA = (destInput.find('A') != std::string::npos);
	int hasD = (destInput.find('D') != std::string::npos);

	// Parse the binary ints into a string.
	std::string code = std::to_string(hasA) + std::to_string(hasD) + std::to_string(hasM);

	// Return binary string.
	return code;
}

std::string Code::comp(std::string compInput)
{
	// the a bit represents if M is part of the computation.
	int a_bit_int = (compInput.find('M') != std::string::npos);
	std::string a_bit = std::to_string(a_bit_int);

	// Parse input by brute force.

	if (compInput == "0")
	{
		return a_bit + "101010";
	}

	else if (compInput == "1")
	{
		return a_bit + "111111";
	}

	else if (compInput == "-1")
	{
		return a_bit + "111010";
	}

	else if (compInput == "D")
	{
		return a_bit + "001100";
	}

	else if (compInput == "A" || compInput == "M")
	{
		return a_bit + "110000";
	}

	else if (compInput == "!D")
	{
		return a_bit + "001101";
	}

	else if (compInput == "!A" || compInput == "!M")
	{
		return a_bit + "110001";
	}

	else if (compInput == "-D")
	{
		return a_bit + "001111";
	}

	else if (compInput == "-A" || compInput == "-M")
	{
		return a_bit + "110011";
	}

	else if (compInput == "D+1")
	{
		return a_bit + "011111";
	}

	else if (compInput == "A+1" || compInput == "M+1")
	{
		return a_bit + "110111";
	}

	else if (compInput == "D-1")
	{
		return a_bit + "001110";
	}

	else if (compInput == "A-1" || compInput == "M-1")
	{
		return a_bit + "110010";
	}

	else if (compInput == "D+A" || compInput == "D+M")
	{
		return a_bit + "000010";
	}

	else if (compInput == "D-A" || compInput == "D-M")
	{
		return a_bit + "010011";
	}
	else if (compInput == "A-D" || compInput == "M-D")
	{
		return a_bit + "000111";
	}
	else if (compInput == "D&A" || compInput == "D&M")
	{
		return a_bit + "000000";
	}
	else if (compInput == "D|A" || compInput == "D|M")
	{
		return a_bit + "010101";
	}

	else
	{
		std::cerr << "Code::comp received unknown input."<<std::endl;
		exit(static_cast<int>(CODE_ERROR::COMP_ERROR));
	}
}


// note this is equivalent to Code::dest
std::string Code::jump(std::string jumpInput)
{

	if (jumpInput == "")
	{
		return "000";
	}
	else if (jumpInput == "JGT")
	{
		return "001";
	}
	else if (jumpInput == "JEQ")
	{
		return "010";
	}
	else if (jumpInput == "JGE")
	{
		return "011";
	}
	else if (jumpInput == "JLT")
	{
		return "100";
	}
	else if (jumpInput == "JNE")
	{
		return "101";
	}
	else if (jumpInput == "JLE")
	{
		return "110";
	}
	else if (jumpInput == "JMP")
	{
		return "111";
	}

	else
	{
		std::cerr << "Code::jump received unknown input.";
		exit(static_cast<int>(CODE_ERROR::JUMP_ERROR));
	}

}

