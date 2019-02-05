#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include <queue>
#include <fstream>

class Util
{
public:
	static std::pair<bool, int> isSymbol(std::string::iterator pos);
	
	static bool isLetter(std::string::iterator pos);
	
	static bool isDigit(std::string::iterator pos);
	
	static bool isMetaChar(std::string::iterator pos);
	
	static bool isMetaEnd(std::string::iterator pos);
	
	static bool isBool(std::string curString);
	
	static bool isPhase(std::string curString);
	
	static bool isDecimal(std::string::iterator pos);
	
	static bool isVarianceNorm(std::string curString);
	
	static bool isDimCheckSum(std::string curString);
	
	static bool isEngine(std::string curString);
	
	static bool isRoundMode(std::string curString);
	
	static bool isPoolMethod(std::string curString);
	
	static std::string to_string(const int& num);
};
#endif
