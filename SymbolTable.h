#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H
#include <string>
#include <queue>
#include "Layer.h"
class SymbolTable
{
public:
	Layer* curLayer;
	std::queue<Layer*> layerQueue;
	std::string scope;
	int numClasses;
	std::vector< std::string> dims;

	SymbolTable()
	{
		scope = "null";
		numClasses = 1000;
	}
	SymbolTable(std::string aScope, int aNumClasses)
	{
		scope = aScope;
		numClasses = aNumClasses;
	}
};
#endif
