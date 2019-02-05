#include <algorithm>
#include <string>
#include <sstream>
#include "Util.h"
#include "Token.h"

std::pair<bool, int> Util::isSymbol(std::string::iterator pos)
{
	switch (*pos)
	{
	case '{':
		return std::pair<bool, int>(true, Token::SYMTYPE_LEFT_BRACE);
	case '}':
		return std::pair<bool, int>(true, Token::SYMTYPE_RIGHT_BRACE);
	case '*':
		return std::pair<bool, int>(true, Token::SYMTYPE_STAR);
	case '/':
		return std::pair<bool, int>(true, Token::SYMTYPE_FORWARD_SLASH);
	case '=':
		return std::pair<bool, int>(true, Token::SYMTYPE_EQUAL);
	case ':':
		return std::pair<bool, int>(true, Token::SYMTYPE_COLON);
	default:
		return std::pair<bool, int>(false, Token::SYMTYPE_INVALID);
	}
}

bool Util::isLetter(std::string::iterator pos)
{
	if ((*pos >= 'a' && *pos <= 'z')
		|| (*pos >= 'A' && *pos <= 'Z')
		|| (*pos == '_'))
	{
		return true;
	}
	return false;
}

bool Util::isDigit(std::string::iterator pos)
{
	if (*pos >= '0' && *pos <= '9')
	{
		return true;
	}
	return false;
}

bool Util::isMetaChar(std::string::iterator pos)
{
	/*std::cout << "Checking for meta\n";*/
	if (*pos == '/' && *(pos + 1) == '/')
	{
		/*std::cout << "It is double //\n";*/
		return true;
	}
	else if (*pos == '/' && *(pos + 1) == '*')
	{
		//Adjust later for ending of comment
		return true;
	}
	else if (*pos == '*')
	{
		return true;
	}
	return false;
}

bool Util::isMetaEnd(std::string::iterator pos)
{
	if (*pos == '*' && *(pos + 1) == '/')
	{
		/*std::cout << "It is double //\n";*/
		return true;
	}
}

bool Util::isBool(std::string curString)
{
	if (!curString.compare("true")
	|| !curString.compare("false"))
	{
		return true;
	}
	return false;	
}

bool Util::isPhase(std::string curString)
{
	if (!curString.compare("TEST")
	|| !curString.compare("TRAIN"))
	{
		return true;
	}
	return false;
}

bool Util::isDecimal(std::string::iterator pos)
{
	if (*pos == '.')
	{
		return true;
	}
	return false;
}

bool Util::isEngine(std::string curString)
{
	if (!curString.compare("DEFAULT")
	|| !curString.compare("CAFFE")
	|| !curString.compare("CUDNN"))
	{
		return true;
	}
	return false;
}

bool Util::isVarianceNorm(std::string curString)
{
	if (!curString.compare("FAN_IN")
	|| !curString.compare("FAN_OUT")
	|| !curString.compare("AVERAGE"))
	{
		return true;
	}
	return false;
}

bool Util::isDimCheckSum(std::string curString)
{
	if (!curString.compare("STRICT")
	|| !curString.compare("PERMISSIVE"))
	{
		return true;
	}
	return false;
}

bool Util::isRoundMode(std::string curString)
{
	if (!curString.compare("CEIL")
	|| !curString.compare("FLOOR"))
	{
		return true;
	}
	return false;
}

bool Util::isPoolMethod(std::string curString)
{
	if (!curString.compare("MAX")
	|| !curString.compare("AVE")
	|| !curString.compare("STOCHASTIC"))
	{
		return true;
	}
	return false;
}

std::string Util::to_string(const int& num)
{
	std::ostringstream outString;
        outString << num;
        return outString.str();
}

