#include "stdafx.h"
#include "SymbolTable.h"

enum class SYMBOLTABLE_ERROR { MISSING_SYMBOL_ERROR = 1 };

SymbolTable::SymbolTable()
{
	// Clear symbol and address tables, add the presets.
	symbol.clear();
	address.clear();
	symbol.push_back("SP");
	address.push_back(0);
	symbol.push_back("LCL");
	address.push_back(1);
	symbol.push_back("ARG");
	address.push_back(2);
	symbol.push_back("THIS");
	address.push_back(3);
	symbol.push_back("THAT");
	address.push_back(4);
	symbol.push_back("SCREEN");
	address.push_back(16384);
	symbol.push_back("KBD");
	address.push_back(24576);

	for (int i = 0;  i < 16; i++)
	{
		std::string sym = "R"+std::to_string(i);
		symbol.push_back(sym);
		address.push_back(i);
	}
}

void SymbolTable::addEntry(std::string sym, int add)
{
	symbol.push_back(sym);
	address.push_back(add);
}

bool SymbolTable::contains(std::string sym)
{
	int length = symbol.size();
	for (int i = 0; i < length; i++)
	{
		if (symbol[i] == sym)
		{
			return true;
		}
	}
	return false;
}

int SymbolTable::GetAddress(std::string sym)
{
	if (!SymbolTable::contains(sym))
	{
		std::cerr << "SymbolTable::GetAddress given string not contained in current symbol table."<< std::endl;
		exit(static_cast<int>(SYMBOLTABLE_ERROR::MISSING_SYMBOL_ERROR));
	}
	else
	{
		int length = symbol.size();
		int index = 0;
		for (int i = 0; i < length; i++)
		{
			if (symbol[i] == sym)
			{
				index = i;
				break;
			}
		}
		return address[index];
	}
	return 0;
}
