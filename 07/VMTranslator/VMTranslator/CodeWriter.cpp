#include "stdafx.h"
#include "CodeWriter.h"
#include <string>

// Note (20/12/2017)
// The convention seems to be that the stack pointer is the address of the first bit of over-writable memory
// the push and add writing has been edited to this convention, but the other methods will need to be checked.


// Note (28/12/2017)
// On comparing out and cmp files, it seems the convention is true = -1 and false = 0.
// Further when asking if less than or greater than we consider the order as SP-2, SP-1, i.e. lt means "is SP-2 < SP-1"?
// These issues were dealt with in the eq/lt/gt part of the writeArithmetic function.
// next step - implement memory segmentation.
enum class CODEWRITER_ERROR { OPEN_STREAM_ERROR=1, UNKNOWN_ARITHMETIC=2, NOT_PUSH_POP =3};

CodeWriter::CodeWriter(string currentDirectory, string file_name)
{
	CodeWriter::setFileName(file_name);
	jump_counter = 0;
	stream.open(currentDirectory + "\\" + filename + ".asm", ios_base::trunc);
	if (!stream.is_open())
	{
		cerr << "Error opening stream in CodeWriter constructor" << endl;
		exit(static_cast<int>(CODEWRITER_ERROR::OPEN_STREAM_ERROR));
	}
}

CodeWriter::~CodeWriter()
{
	stream.close();
}

void CodeWriter::setFileName(string file_name)
{
	filename = file_name;
}

void CodeWriter::writeArithmetic(string operation)
{
	if (operation == "add")
	{
		// get stack pointer
		stream << "@SP" << endl;
		// get first variable
		stream << "A=M-1" << endl;
		stream << "D=M" << endl;
		// update address
		stream << "A=A-1" << endl;
		// add second variable, store in previous address
		stream << "M = D+M" << endl;
		// update stack pointer.
		stream << "D=A" << endl;
		stream << "@SP" << endl;
		stream << "M=D+1" << endl;

	}

	else if (operation == "sub")
	{
		// get stack pointer
		stream << "@SP" << endl;
		// get first variable
		stream << "A=M-1" << endl;
		stream << "D=M" << endl;
		// update address
		stream << "A=A-1" << endl;
		// add second variable, store in previous address
		stream << "M = M-D" << endl;
		// update stack pointer.
		stream << "D=A" << endl;
		stream << "@SP" << endl;
		stream << "M=D+1" << endl;
	}

	else if (operation == "neg")
	{
		// get stack pointer
		stream << "@SP" << endl;
		// get first variable
		stream << "A=M-1" << endl;
		stream << "M = -M" << endl;
	}

	else if (operation == "eq" || operation == "lt" || operation == "gt")
	{
		string jump_name = "EQUAL" + to_string(jump_counter);
		jump_counter++;
		string jump = "J";
		if (operation == "eq")
		{
			jump = jump + "EQ";
		}
		if (operation == "lt")
		{
			jump = jump + "LT";
		}
		if (operation == "gt")
		{
			jump = jump + "GT";
		}
		// get stack pointer
		stream << "@SP" << endl;
		// get first variable
		stream << "A=M-1" << endl;
		stream << "D=M" << endl;
		// update address
		stream << "A=A-1" << endl;
		// compute if equal
		stream << "D = M-D" << endl;
		stream << "M = -1" << endl;
		stream << "@"+jump_name << endl;
		stream << "D;"+jump << endl;
		// if not equal, set M = -1 at the stack tip
		stream << "@SP" << endl;
		stream << "A = M-1" << endl;
		stream << "A = A-1" << endl;
		stream << "M=0" << endl;
		stream << "("+jump_name + ")" << endl;
		// update stack pointer
		stream << "@SP" << endl;
		stream << "A=M" << endl;
		stream << "D = A-1" << endl;
		stream << "@SP" << endl;
		stream << "M=D" << endl;
	}

	else if (operation == "and" || operation == "or")
	{
		string op;
		if (operation == "and")
		{
			op = "&";
		}
		if (operation == "or")
		{
			op = "|";
		}
		// get stack pointer
		stream << "@SP" << endl;
		// get first variable
		stream << "A=M-1" << endl;
		stream << "D=M" << endl;
		// update address
		stream << "A=A-1" << endl;
		// compute operation
		stream << "M = D" + op + "M" << endl;
		// update stack
		stream << "D = A" << endl;
		stream << "@SP" << endl;
		stream << "M = D+1" << endl;
	}

	else if (operation == "not")
	{
		// get stack pointer
		stream << "@SP" << endl;
		// get first variable
		stream << "A=M-1" << endl;
		// compute not
		stream << "M = !M" << endl;
	}

	else
	{
		cerr << "Unknown operation in CodeWriter::writeArithmetic." << endl;
		exit(static_cast<int>(CODEWRITER_ERROR::UNKNOWN_ARITHMETIC));
	}


}

// This seems to work but hasn't handled temp, pointer, or static. These aren't really explained in the slides, so maybe check out other implementations.

void CodeWriter::writePushPop(command_type cType, string memSegment, int num)
{
	if (cType != command_type::C_POP && cType != command_type::C_PUSH)
	{
		cerr << "Error, CodeWriter::writePushPop used on command that is not push or pop." << endl;
		exit(static_cast<int>(CODEWRITER_ERROR::NOT_PUSH_POP));
	}
	else
	{
		// memory segments : static (R16-255), this, local, argument, that (heap, 2048 onward)
		// constant (virtual), pointer (later), temp
		if (memSegment == "constant" && cType == command_type::C_PUSH)
		{
			string num_str = to_string(num);
			// store num in memory
			stream << "@" + num_str << endl;
			stream << "D = A" << endl;
			// get stack pointer - address of empty/overwritable memory
			stream << "@SP" << endl;
			stream << "A = M" << endl;
			// set memory
			stream << "M=D" << endl;
			// update stack pointer
			stream << "D=A" << endl;
			stream << "@SP" << endl;
			stream << "M=D+1" << endl;
		}
		string memory_shortcut = "";

		if (memSegment == "local")
		{
			memory_shortcut = "LCL";
		}
		if (memSegment == "argument")
		{
			memory_shortcut = "ARG";
		}
		if (memSegment == "this")
		{
			memory_shortcut = "THIS";
		}
		if (memSegment == "that")
		{
			//cout << "That detected" << endl;
			memory_shortcut = "THAT";
		}
		if (memory_shortcut != "")
		{
			if (cType == command_type::C_PUSH)
			{
				string num_str = to_string(num);
				int counter = num;
				// get local variable
				stream << "@" + memory_shortcut << endl;
				stream << "A = M" << endl;
				while (counter > 0)
				{
					stream << "A = A+1" << endl;
					counter--;
				}
				stream << "D = M" << endl;
				// get stack
				stream << "@SP" << endl;
				stream << "A = M" << endl;
				stream << "M = D" << endl;
				// update stack pointer
				stream << "@SP" << endl;
				stream << "A = M" << endl;
				stream << "D = A + 1" << endl;
				stream << "@SP" << endl;
				stream << "M = D" << endl;
			}
			else if (cType == command_type::C_POP)
			{
				string num_str = to_string(num);
				int counter = num;
				// get variable off top of stack
				stream << "@SP" << endl;
				stream << "A = M" << endl;
				stream << "A = A-1" << endl;
				stream << "D = M" << endl;
				// get location in memory
				stream << "@" + memory_shortcut << endl;
				stream << "A = M" << endl;
				while (counter > 0)
				{
					stream << "A = A+1" << endl;
					counter--;
				}
				stream << "M = D" << endl;
				// update stack pointer
				stream << "@SP" << endl;
				stream << "A = M" << endl;
				stream << "D = A-1" << endl;
				stream << "@SP" << endl;
				stream << "M = D" << endl;

			}
		}

	}
}
