#include "Scanner.h"
#include "Token.h"
#include "Util.h"
/*#include <iostream>*/

Scanner::Scanner(const char* inputProgram)
{
	inpPgm.open(inputProgram, std::ifstream::in);
	curLine.erase();
}

bool Scanner::hasNextLine()
{
	curLine.erase();
	if (std::getline(inpPgm, curLine))
	{
		lineIt = curLine.begin();
		return true;
	}
	else
	{
		curLine.erase();
		return false;
	}
}

bool Scanner::hasNextToken()
{
	if (curLine.empty() || lineIt == curLine.end())
	{
		if (!hasNextLine())
		{
			return false;
		}
		/*std::cout << "\t\tRead next Line\n";*/
	}

	curToken = new Token();
	populateToken();
	return true;
}

Token* Scanner::getNextToken()
{
	return curToken;
}

void Scanner::populateToken()
{
	while (lineIt != curLine.end())
	{
		if (curToken->getID() == Token::IDTYPE_METASTATEMENT)
		{
			/*std::cout << "METASTATEMENT\t" << *lineIt << "\n";*/
			curToken->push(*lineIt);
		}
		else if (curToken->getID() == Token::IDTYPE_STRING)
		{
			/*std::cout << "STRING\t" << *lineIt << "\n";*/
			if (*lineIt != '"')
			{
				curToken->push(*lineIt);
			}
			else
			{
				curToken->push(*lineIt);
				++lineIt;
				curToken->isStringComplete = true;
				break;
			}
		}
		
		else if (Util::isDigit(lineIt))
		{
			/*std::cout << "DIGIT\t" << *lineIt << "\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY)
			{
				curToken->setID(Token::IDTYPE_INT);
				curToken->push(*lineIt);
			}
			else if (curToken->getID() == Token::IDTYPE_INT 
			|| curToken->getID() == Token::IDTYPE_DECIMAL
			|| curToken->getID() == Token::IDTYPE_IDENTIFIER)
			{
				curToken->push(*lineIt);
			}
			else
			{
				break;
			}
		}
		else if (Util::isLetter(lineIt))
		{
			/*std::cout << "LETTER\t" << *lineIt << "\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY)
			{
				curToken->setID(Token::IDTYPE_IDENTIFIER);
				curToken->push(*lineIt);
			}
			else if (curToken->getID() == Token::IDTYPE_IDENTIFIER)
			{
				curToken->push(*lineIt);
			}
			else
			{
				break;
			}
		}
		else if (Util::isDecimal(lineIt))
		{
			if (curToken->getID() == Token::IDTYPE_INT)
			{
				curToken->setID(Token::IDTYPE_DECIMAL);
				curToken->push(*lineIt);
			}
			else
				break;
		}
		else if (Util::isSymbol(lineIt).first)
		{
			/*std::cout << "SYMBOL\t" << *lineIt  << "\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY)				/*Start of a symbol so intialize new token*/
			{
				if (Util::isMetaChar(lineIt))				/*If the symbol is a meta character, change the token to meta character*/
				{
					curToken->setID(Token::IDTYPE_METASTATEMENT);
					curToken->push(*lineIt);
				}
				else if (Util::isMetaEnd(lineIt) && curToken->getID() == Token::IDTYPE_METASTATEMENT)
				{
					curToken->push(*lineIt);
					break;
				}
				else
				{
					curToken->setID(Token::IDTYPE_SYMBOL);			/*If the symbol is a not meta character, initialize the token to symbol*/
					curToken->push(*lineIt);
					
					curToken->setSymType(Util::isSymbol(lineIt).second);

					++lineIt;
					break;
				}
			}
			else
			{
				break;
			}
		}
		else if (*lineIt == '"')
		{
			/*std::cout << "QUOTES\t" << *lineIt << "\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY)				/*Start of a string so intialize new token*/
			{
				curToken->setID(Token::IDTYPE_STRING);
				curToken->push(*lineIt);
				curToken->isString = true;
			}
			else
			{
				break;								/*Start of string so save the previous token*/
			}
		}
		else if (*lineIt == '\t' || *lineIt == ' ' || *lineIt == '\r' || *lineIt == '\n')
		{
			/*std::cout << "TABSPACE\t[" << *lineIt << "]\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY || curToken->getID() == Token::IDTYPE_TABSPACE)
			{
				curToken->setID(Token::IDTYPE_TABSPACE);
				curToken->push(*lineIt);
			}
			else
			{
				break;
			}
		}
		else
		{									/* This branch will execute when the character is not a digit,
											 * letter, symbol, meta character, or part of a string
											 */
			/*std::cout<<"ELSE PART\t [" << *lineIt << "]\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY)
			{
				++lineIt;
				curToken->setID(Token::IDTYPE_INVALID);
			}
			else if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isBool(curToken->getTokenName()))
			{
				curToken->setID(Token::IDTYPE_BOOL);
				break;
			}
			else if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isPhase(curToken->getTokenName()))
			{
				curToken->setID(Token::IDTYPE_PHASE);
				break;
			}
			else if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isEngine(curToken->getTokenName()))
			{
				curToken->setID(Token::IDTYPE_ENGINE);
				break;
			}
			else if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isVarianceNorm(curToken->getTokenName()))
			{
				curToken->setID(Token::IDTYPE_VARIANCENORM);
				break;
			}
			else if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isDimCheckSum(curToken->getTokenName()))
			{
				curToken->setID(Token::IDTYPE_DIMCHECKSUM);
				break;
			}
			else if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isRoundMode(curToken->getTokenName()))
			{
				curToken->setID(Token::IDTYPE_ROUNDMODE);
				break;
			}
			else if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isPoolMethod(curToken->getTokenName()))
			{
				curToken->setID(Token::IDTYPE_ROUNDMODE);
				break;
			}
			break;
		}
		if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isBool(curToken->getTokenName()))
		{
			curToken->setID(Token::IDTYPE_BOOL);
		}
		else if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isPhase(curToken->getTokenName()))
		{
			curToken->setID(Token::IDTYPE_PHASE);
		}
		else if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isEngine(curToken->getTokenName()))
		{
			curToken->setID(Token::IDTYPE_ENGINE);
		}
		else if (curToken->getID() == Token::IDTYPE_IDENTIFIER && Util::isVarianceNorm(curToken->getTokenName()))
		{
			curToken->setID(Token::IDTYPE_VARIANCENORM);
		}
		++lineIt;
	}

	if (curToken->isString && !curToken->isStringComplete)
	{
		/*std::cout<<"INCOMPLETE STRING\n";*/
		curToken->setID(Token::IDTYPE_INVALID);
	}
}
