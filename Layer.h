#ifndef _LAYER_H
#define _LAYER_H

#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <string>
class Layer
{
public:
	std::string name;
	std::vector< std::pair<std::string, std::string> > fields;
	std::string layerType;
	std::vector< std::string> bottoms;
	std::string top;
	std::vector< std::string> branchValues;

	Layer()
	{
		name = "null";
		layerType = "null";
		top = "null";
	}
	
	void setName(std::string aName)
	{
		name = aName;
	}
	
	void setType(std::string aType)
	{
		layerType = aType;
	}

	friend std::ofstream& operator<<(std::ofstream& outFile, Layer& printLayer);
};

#endif
