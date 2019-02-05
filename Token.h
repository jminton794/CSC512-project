#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

class Token
{
private:
	std::string myToken;
	int id;
	int symType;

public:
	bool isString;
	bool isStringComplete;

public:
	static enum IDTYPE{
		IDTYPE_IDENTIFIER,
		IDTYPE_INT,
		IDTYPE_DECIMAL,
		IDTYPE_SYMBOL,
		IDTYPE_STRING,
		IDTYPE_METASTATEMENT,
		IDTYPE_TABSPACE,
		IDTYPE_INVALID,
		IDTYPE_BOOL,
		IDTYPE_PHASE,
		IDTYPE_ENGINE,
		IDTYPE_VARIANCENORM,
		IDTYPE_DIMCHECKSUM,
		IDTYPE_ROUNDMODE,
		IDTYPE_POOLMETHOD,
		IDTYPE_EMPTY
	}ID;

	static enum SYMBOLTYPE{
		SYMTYPE_LEFT_BRACE,
		SYMTYPE_RIGHT_BRACE,
		SYMTYPE_STAR,
		SYMTYPE_FORWARD_SLASH,
		SYMTYPE_COLON,
		SYMTYPE_EQUAL,
		SYMTYPE_INVALID
	}STYPE;

public:
	Token();
	
	void push(char c);

	void setID(int newID);

	int getID();

	void setSymType(int newSymType);

	int getSymType();

	std::string getTokenName();

	char last();
};
#endif
