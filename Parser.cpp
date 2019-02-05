#include <vector>
#include "Parser.h"
#include "Util.h"
#include <iostream>
Token* Parser::nextToken()
{
	++tokenIT;

	while ((tokenIT != tokenVector->end()) 
		&& ((*tokenIT)->getID() == Token::IDTYPE_TABSPACE 
		|| (*tokenIT)->getID() == Token::IDTYPE_METASTATEMENT 
		|| (*tokenIT)->getID() == Token::IDTYPE_EMPTY))
	{
		++tokenIT;
	}

	if (tokenIT == tokenVector->end())
	{
		return NULL;
	}

	/*std::cout << "Token : " << (*tokenIT)->getTokenName() << std::endl;*/
	return *tokenIT;
}

Token* Parser::curToken()
{
	if (tokenIT == tokenVector->end())
	{
		return NULL;
	}

	return *tokenIT;
}

void Parser::addToken(Token *newToken)
{
	tokenVector->push_back(newToken);
}


bool Parser::parseProgram()
{
	std::cout << "Parsing begun" << std::endl;
	tokenIT = tokenVector->begin();
	while ((tokenIT != tokenVector->end())
		&& ((*tokenIT)->getID() == Token::IDTYPE_TABSPACE
		|| (*tokenIT)->getID() == Token::IDTYPE_METASTATEMENT
		|| (*tokenIT)->getID() == Token::IDTYPE_EMPTY))
	{
		++tokenIT;
	}
	if (tokenIT != tokenVector->end())
	{
		std::cout << "Grammar started" << std::endl;
		bool temp = myGrammar->start();
		/*std::cout << std::endl;
		while (tokenIT != tokenVector->end())
		{
			std::cout << (*tokenIT)->getTokenName();
			++tokenIT;
		}*/
		std::cout << "Grammar ended" << std::endl;
		return temp;
	}
	return true;
}

SymbolTable* Parser::getSymbolTable()
{
	return mySymTab;
}

Parser::Parser(Grammar *newGrammar
				, SymbolTable* newSymTab) 
				: myGrammar(newGrammar)
				, tokenVector(new std::vector<Token*>())
				, mySymTab(newSymTab)
{
	newGrammar->instantiateParser(this);
}

Parser::~Parser()
{
	for (std::vector<Token *>::iterator it = tokenVector->begin(); it != tokenVector->end(); ++it)
	{
		delete *it;
	}

	delete tokenVector;
}
