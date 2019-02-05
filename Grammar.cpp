#include "Grammar.h"
#include "Layer.h"
#include "Token.h"
#include "SymbolTable.h"
#include "Parser.h"
#include <cstring>
#include <cstdlib>
#include <string>
#include <queue>
#include <vector>
#include <iostream>

bool Grammar::start()
{
	/**Production:	<start> --> <net param> <layer list>
	 */
	 if (netParam())
	 {
		 return layer();
	 }
}

bool Grammar::netParam()
{
	/** Production: <net param> --> <name> <input> <input shape> <input dim> <force backward> <state> <debug info>
	 */
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "name")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "input")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "input_shape")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "input_dim")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "force_backward")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "state")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "debug_info")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "name")
			&& !name())
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "input")
			&& !input())
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "input_shape")
			&& !inputShape())
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "input_dim")
			&& !inputDim())
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "force_backward")
			&& !force_backward())
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "state")
			&& !state())
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "debug_info")
			&& !debug_info())
		{
			return false;
		}
	}
	if (!strcmp(parse->curToken()->getTokenName().c_str(), "layer"))
	{
		return true;
	}
	return false;
}

bool Grammar::name()
{
	/** Production:	<name> --> name : STRING
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "name")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_STRING)
		{
			parse->getSymbolTable()->scope = parse->curToken()->getTokenName().c_str();
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::input()
{
	/** Production:	<input> --> input : STRING
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "input")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_STRING)
		{
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::inputShape()
{
	/** Production: <input shape> --> <input shape0> <input shape>
	 * 									| EPSILON
	 */
	 if (inputShape0())
	 {
		 return inputShape();
	 }
	 return true;
}

bool Grammar::inputShape0()
{
	/** Production:	<input0> --> input_shape { <BlobShape> }
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "input_shape")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& BlobShape()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
		{
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::BlobShape()
{
	/** Production: <BlobShape> --> <dim> <BlobShape>
	 * 								| EPSILON
	 */
	 if (dim())
	 {
		 return BlobShape();
	 }
	 return true;
}

bool Grammar::dim()
{
	/** Production:	<dim> --> dim : INT
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "dim")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_INT)
		{
			parse->getSymbolTable()->dims.push_back(parse->curToken()->getTokenName().c_str());
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::inputDim()
{
	/** Production: <input dim> --> <input dim0> <input dim>
	 * 							| EPSILON
	 */
	 if (inputDim0())
	 {
		 return input();
	 }
	 return true;
}

bool Grammar::inputDim0()
{
	/** Production:	<input dim0> --> input_dim : INT
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "input_dim")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_INT)
		{
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::force_backward()
{
	/** Production:	<force backward> --> force_backward : BOOL
	 */
	 if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "force_backward")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_BOOL)
		{
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::state()
{
	/** Production:	<state> --> state { <NetState()> }
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "input_shape")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& NetState()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
		{
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::NetState()
{
	/** Production:	<NetState> --> <phase> <level> <stage>
	 */
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "phase")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "level")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "stage")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "phase")
			&& !phase())
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "level")
			&& !level())
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "stage")
			&& !stage())
		{
			return false;
		}
	}
}

bool Grammar::stage()
{
	/**Production: <stage> --> <stage0> <stage>
	*/
	if (stage0())
	{
		return stage();
	}
	return true;
}

bool Grammar::level()
{
	/**Production: <level> --> level : STRING
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "level")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::stage0()
{
	/**Production: <stage0> --> stage : STRING
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "stage")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_STRING)
	{
		parse->nextToken();
		return true;
	}
	return false;
}


bool Grammar::debug_info()
{
	/** Production:	<debug info> --> debug_info : BOOL
	 */
	 if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "debug_info")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_BOOL)
		{
			parse->nextToken();
			return true;
		}
		return false;
}

//LayerList
bool Grammar::layer()
{
	/** Production: <layer> --> <layer0> <layer>
	*/
	Layer aLayer;
	while(layer0(&aLayer))
	{
		std::cout << "Layer added" << std::endl;
		parse->getSymbolTable()->layerQueue.push(&aLayer);
	}
	return true;
}

bool Grammar::layer0(Layer* aLayer)
{
	/** Production:	<layer0> --> layer { <name> <type> <bottom> <top> <phase> <loss weight> <param> <blobs> 
	* <propagate down> <include or exclude> <other> }
	*/
	std::cout << "Layer0 start" << std::endl;
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "layer")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& layerParam(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
		{
			std::cout << "Layer worked properly" << std::endl;
			return true;
		}
		return false;
}

bool Grammar::layerParam(Layer* aLayer)
{
	int i = 0;
	std::cout << "LayerParam reached" << std::endl;
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "name")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "type")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "bottom")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "top")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "phase")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "loss_weight")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "param")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "blobs")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "propagate_down")))
	{
		std::cout << parse->curToken()->getTokenName().c_str() << std::endl;
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "name")
			&& !name(aLayer))
		{
			std::cout << "Error in name" << std::endl;
			return false;
		}
		std::cout << "Exited name line" << std::endl;
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "type")
			&& !type(aLayer))
		{
			std::cout << "Error in type" << std::endl;
			return false;
		}
		else if(!strcmp(parse->curToken()->getTokenName().c_str(), "bottom")
			&& !bottom(aLayer))
		{
			return false;
		}
		else if(!strcmp(parse->curToken()->getTokenName().c_str(), "top")
			&& !top(aLayer))
		{
			return false;
		}
		else if(!strcmp(parse->curToken()->getTokenName().c_str(), "phase")
			&& !phase(aLayer))
		{
			return false;
		}
		else if(!strcmp(parse->curToken()->getTokenName().c_str(), "loss_weight")
			&& !loss_weight(aLayer))
		{
			return false;
		}
		else if(!strcmp(parse->curToken()->getTokenName().c_str(), "param")
			&& !param(aLayer))
		{
			return false;
		}
		else if(!strcmp(parse->curToken()->getTokenName().c_str(), "blobs")
			&& !height(aLayer))
		{
			return false;
		}
		else if(!strcmp(parse->curToken()->getTokenName().c_str(), "propagate_down")
			&& !propagate_down(aLayer))
		{
			return false;
		}
		else
			continue;
		++i;
	}
	std::cout << "Number of times looped: " << i << std::endl;
	return other(aLayer);
}

bool Grammar::name(Layer* aLayer)
{
	/**Production: <name> --> name : STRING
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "name")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_STRING)
	{
		std::cout << "name reached" << std::endl;
		aLayer->name = parse->curToken()->getTokenName().c_str();
		parse->nextToken();
		std::cout << "token after name: " << parse->curToken()->getTokenName().c_str() << std::endl;
		return true;
	}
	else 
	{
		std::cout << "Error in comparison flow." << std::endl;
		return false;
	}
}

bool Grammar::type(Layer* aLayer)
{
	/**Production: <type> --> type : STRING
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "type")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_STRING)
	{
		aLayer->layerType = parse->curToken()->getTokenName().c_str();
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::bottom(Layer* aLayer)
{
	/**Production: <bottom> --> <bottom0> <bottom>
	*/
	if (bottom0(aLayer))
	{
		return bottom(aLayer);
	}
	return true;
}

bool Grammar::bottom0(Layer* aLayer)
{
	/**Production: <name> --> bottom : STRING
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "bottom")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_STRING)
	{
		aLayer->bottoms.push_back(parse->curToken()->getTokenName().c_str());
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::top(Layer* aLayer)
{
	/**Production: <top> --> <top0> <top>
	*/
	if (top0(aLayer))
	{
		return top(aLayer);
	}
	return true;
}

bool Grammar::top0(Layer* aLayer)
{
	/**Production: <top0> --> top : STRING
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "top")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_STRING)
	{
		aLayer->top = parse->curToken()->getTokenName().c_str();
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::loss_weight(Layer* aLayer)
{
	/**Production: <loss_weight> --> <loss_weight0> <loss_weight>
	*/
	if (loss_weight0(aLayer))
	{
		return loss_weight(aLayer);
	}
	return true;
}

bool Grammar::loss_weight0(Layer* aLayer)
{
	/**Production: <loss_weight0> --> loss_weight : DECIMAL
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "loss_weight")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("loss_weight", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::propagate_down(Layer* aLayer)
{
	/**Production: <loss_weight> --> <loss_weight0> <loss_weight>
	*/
	if (propagate_down0(aLayer))
	{
		return propagate_down(aLayer);
	}
	return true;
}

bool Grammar::propagate_down0(Layer* aLayer)
{
	/**Production: <propagate_down0> --> propagate_down : DECIMAL
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "propagate_down")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_BOOL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("propagate_down", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::phase()
{
	/**Production: <type> --> type : STRING
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "phase")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_PHASE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::phase(Layer* aLayer)
{
	/**Production: <type> --> type : STRING
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "phase")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_PHASE)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("phase", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::param(Layer* aLayer)
{
	/**
	* Production:	<param> --> param { <Layer* aLayer> }
   	*  				| EPSILON
	*/
	/*std::cout << "convolution_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& ParamSpec(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::include(Layer* aLayer)
{
	/**
	* Production:	<include> --> include { <NetStateRule> }
   	*  				| EPSILON
	*/
	/*std::cout << "include" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "include")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& NetStateRule(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::exclude(Layer* aLayer)
{
	/**
	* Production:	<exclude> --> exclude { <NetStateRule> }
   	*  				| EPSILON
	*/
	/*std::cout << "exclude" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "exclude")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& NetStateRule(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::NetStateRule(Layer* aLayer)
{
	/**
	* Production:	<NetStateRule> --> <phase> <min_level> <max_level> <stage> <not_stage>
   	*  			
	*/
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "phase")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "min_level")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "max_level")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "stage")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "not_stage")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "phase")
			&& !phase(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "min_level")
			&& !min_level(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "max_level")
			&& !max_level(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "stage")
			&& !stage(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "not_stage")
			&& !not_stage(aLayer))
		{
			return false;
		}
	}
	return true;
}

bool Grammar::stage(Layer* aLayer)
{
	/**Production: <stage> --> <stage0> <stage>
	*/
	if (stage0(aLayer))
	{
		return stage(aLayer);
	}
	return true;
}

bool Grammar::stage0(Layer* aLayer)
{
	/**Production: <stage0> --> stage : STRING
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "stage")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_STRING)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("stage", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::not_stage(Layer* aLayer)
{
	/**Production: <not_stage> --> <not_stage0> <not_stage>
	*/
	if (not_stage0(aLayer))
	{
		return not_stage(aLayer);
	}
	return true;
}

bool Grammar::not_stage0(Layer* aLayer)
{
	/**Production: <not_stage0> --> not_stage : STRING
	*/
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "not_stage")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_STRING)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("not_stage", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}


bool Grammar::blobs(Layer* aLayer)
{
	/**Production: <blobs> --> <blobs0> <blobs>
	*/
	if (blobs0(aLayer))
	{
		return blobs(aLayer);
	}
	return true;
}

bool Grammar::blobs0(Layer* aLayer)
{
	/**
	* Production:	<blobs> --> blobs { <BlobProto> }
   	*  				| EPSILON
	*/
	/*std::cout << "convolution_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& BlobProto(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::BlobProto(Layer* aLayer)
{
	/**
	* Production:	<BlobProto> --> <shape> <data> <diff> <double_data> <double_diff> <num> <channels> <height> <width>
	*/
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "shape")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "data")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "diff")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "double_data")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "double_diff")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "num")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "channels")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "height")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "width")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "shape")
			&& !shape(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "data")
			&& !data(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "diff")
			&& !diff(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "double_data")
			&& !double_data(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "double_diff")
			&& !double_diff(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "num")
			&& !num(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "channels")
			&& !channels(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "height")
			&& !height(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "width")
			&& !width(aLayer))
		{
			return false;
		}
	}
	return true;
}

bool Grammar::min_level(Layer* aLayer)
{
	/** Production:	<min_level> --> min_level : INT
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "min_level")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_INT)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("min_level", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::max_level(Layer* aLayer)
{
	/** Production:	<max_level> --> max_level : INT
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "max_level")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_INT)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("max_level", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::data(Layer* aLayer)
{
	/** Production: <data> --> <data0> <data>
	 * 							| EPSILON
	 */
	 if (data0(aLayer))
	 {
		 return data(aLayer);
	 }
	 return true;
}

bool Grammar::data0(Layer* aLayer)
{
	/** Production:	<data0> --> data : DECIMAL
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "data")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("data", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::diff(Layer* aLayer)
{
	/** Production: <diff> --> <diff0> <diff>
	 * 							| EPSILON
	 */
	 if (diff0(aLayer))
	 {
		 return diff(aLayer);
	 }
	 return true;
}

bool Grammar::diff0(Layer* aLayer)
{
	/** Production:	<diff0> --> diff : DECIMAL
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "diff")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("diff", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::double_data(Layer* aLayer)
{
	/** Production: <double_data> --> <double_data0> double_<data>
	 * 							| EPSILON
	 */
	 if (double_data0(aLayer))
	 {
		 return double_data(aLayer);
	 }
	 return true;
}

bool Grammar::double_data0(Layer* aLayer)
{
	/** Production:	<double_data0> --> double_data : DECIMAL
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "double_data")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("double_data", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::double_diff(Layer* aLayer)
{
	/** Production: <double_diff> --> <double_diff0> <double_diff>
	 * 							| EPSILON
	 */
	 if (double_diff0(aLayer))
	 {
		 return double_diff(aLayer);
	 }
	 return true;
}

bool Grammar::double_diff0(Layer* aLayer)
{
	/** Production:	<double_diff0> --> double_diff : DECIMAL
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "double_diff")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("double_diff", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::num(Layer* aLayer)
{
	/** Production:	<num> --> num : INT
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "num")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_INT)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("num", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::channels(Layer* aLayer)
{
	/** Production:	<channels> --> channels : INT
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "channels")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_INT)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("channels", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::height(Layer* aLayer)
{
	/** Production:	<height> --> height : INT
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "height")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_INT)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("height", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::width(Layer* aLayer)
{
	/** Production:	<width> --> width : INT
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "width")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_INT)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("width", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::ParamSpec(Layer* aLayer)
{
	/**
	* Production:	<ParamSpec> --> <name> <share_mode> <lr_mult> <decay_mult>
	*/
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "name")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "share_mode")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "lr_mult")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "decay_mult")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "name")
			&& !name(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "share_mode")
			&& !share_mode(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "lr_mult")
			&& !lr_mult(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "decay_mult")
			&& !decay_mult(aLayer))
		{
			return false;
		}
	}
}

bool Grammar::share_mode(Layer* aLayer)
{
	//Production: <share_mode> --> share_mode: DIMCHECKSUM
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "share_mode")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DIMCHECKSUM)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("share_mode", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::lr_mult(Layer* aLayer)
{
	//Production: <lr_mult> --> lr_mult: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "lr_mult")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("lr_mult", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::decay_mult(Layer* aLayer)
{
	//Production: <decay_mult> --> decay_mult: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "decay_mult")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("decay_mult", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::other(Layer* aLayer)
{
	/**
	* Production:	<other> --> <convolution_param>
	*			| <batch_norm_param>
	*			| <scale_param>
	*			| <relu_param>
	*			| <pooling_param>
	*			| <concat_param>
	*			| <dropout_param>
	*			| <reshape_param>
	*			| <softmax_param>
	*			| EPSILON
	*/
	std::cout << "Other reached" << std::endl;
	if (!strcmp(parse->curToken()->getTokenName().c_str(), "convolution_param"))
	{
		std::cout << "reaches convolution param" << std::endl;
		return convolutionParam(aLayer);
	}
	else if (!strcmp(parse->curToken()->getTokenName().c_str(), "batch_norm_param"))
	{
		return batchNormParam(aLayer);
	}
	else if (!strcmp(parse->curToken()->getTokenName().c_str(), "scale_param"))
	{
		return scaleParam(aLayer);
	}
	else if (!strcmp(parse->curToken()->getTokenName().c_str(), "relu_param"))
	{
		return reluParam(aLayer);
	}
	else if (!strcmp(parse->curToken()->getTokenName().c_str(), "pooling_param"))
	{
		return poolingParam(aLayer);
	}
	else if (!strcmp(parse->curToken()->getTokenName().c_str(), "concat_param"))
	{
		return concatParam(aLayer);
	}
	else if (!strcmp(parse->curToken()->getTokenName().c_str(), "dropout_param"))
	{
		return dropoutParam(aLayer);
	}
	else if (!strcmp(parse->curToken()->getTokenName().c_str(), "reshape_param"))
	{
		return reshapeParam(aLayer);
	}
	else if (!strcmp(parse->curToken()->getTokenName().c_str(), "softmax_param"))
	{
		return softmaxParam(aLayer);
	}
	return true;
}
bool Grammar::convolutionParam(Layer* aLayer)
{
	/**
	* Production:	<convolution_param> --> convolution_param { <convolution_param0> }
   	*  				| EPSILON
	*/
	/*std::cout << "convolution_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "convolution_param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& convolutionParam0(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::convolutionParam0(Layer* aLayer)
{
	/**
	* Production:	<convolution_param0> --> <num_output> <kernel_size> <weight_filler> <bias_term> <pad> <stride> <group>
	*/
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "num_output")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "bias_term")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "pad")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "kernel_size")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "stride")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "pad_h")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "pad_w")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "kernel_h")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "kernel_w")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "stride_h")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "stride_w")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "group")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "weight_filler")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "dilation")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "bias_filler")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "engine")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "axis")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "force_nd_im2col")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "num_output")
			&& !num_output(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "bias_term")
			&& !bias_term(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "pad")
			&& !pad(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "kernel_size")
			&& !kernel_size(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "stride")
			&& !stride(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "pad_h")
			&& !pad_h(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "pad_w")
			&& !pad_w(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "kernel_h")
			&& !kernel_h(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "kernel_w")
			&& !kernel_w(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "stride_h")
			&& !stride_h(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "stride_w")
			&& !stride_w(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "group")
			&& !group(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "weight_filler")
			&& !weight_filler(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "dilation")
			&& !dilation(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "bias_filler")
			&& !bias_filler(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "engine")
			&& !engine(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "axis")
			&& !axis(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "force_nd_im2col")
			&& !force_nd_im2col(aLayer))
		{
			return false;
		}
	}
	return true;
}

bool Grammar::batchNormParam(Layer* aLayer)
{
	/**
	* Production:	<batch_norm_param> --> batch_norm_param { <batch_norm_param0> }
   	*  				| EPSILON
	*/
	/*std::cout << "batch_norm_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "batch_norm_param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& batchNormParam0(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::batchNormParam0(Layer* aLayer)
{
	/**
	* Production:	<batch_norm_param0> --> <use_global_stats> <moving_average_fraction> <eps>
	*/
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "use_global_stats")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "moving_average_fraction")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "eps")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "use_global_stats")
			&& !use_global_stats(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "moving_average_fraction")
			&& !moving_average_fraction(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "eps")
			&& !eps(aLayer))
		{
			return false;
		}
	}
	return true;
}

bool Grammar::scaleParam(Layer* aLayer)
{
	/**
	* Production:	<scale_param> --> scale_param { <scale_param0> }
   	*  				| EPSILON
	*/
	/*std::cout << "scale_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "scale_param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& scaleParam0(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::scaleParam0(Layer* aLayer)
{
	/**
	* Production:	<scale_param0> --> <axis> <num_axes> <filler> <bias_term> <bias_filler>
	*/
	/*std::cout << "scaleParam0" << std::endl;*/
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "axis")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "num_axes")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "filler")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "bias_term")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "bias_filler")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "axis")
			&& !axis(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "num_axes")
			&& !num_axes(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "filler")
			&& !filler(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "bias_term")
			&& !bias_term(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "bias_filler")
			&& !bias_filler(aLayer))
		{
			return false;
		}
	}
	return true;
}



bool Grammar::reluParam(Layer* aLayer)
{
	/**
	* Production:	<relu_param> --> relu_param { <relu_param0> }
   	*  				| EPSILON
	*/
	/*std::cout << "relu_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "relu_param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& reluParam0(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::reluParam0(Layer* aLayer)
{
	/**
	* Production:	<relu_param0> --> <negative_slope> <engine>
	*				| <engine> <negative_slope>
	*/
	if (parse->curToken())
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "negative_slope"))
		{
			if(negative_slope(aLayer))
			{
				if (!strcmp(parse->curToken()->getTokenName().c_str(), "engine"))
				{
					return engine(aLayer);
				}
				return true;
			}
		}
		else if(!strcmp(parse->curToken()->getTokenName().c_str(), "engine"))
		{
			if(engine(aLayer))
			{
				if (!strcmp(parse->curToken()->getTokenName().c_str(), "negative_slope"))
				{
					return negative_slope(aLayer);
				}
				return true;
			}
		}
	}
	return false;
}

bool Grammar::poolingParam(Layer* aLayer)
{
	/**
	* Production:	<pooling_param> --> pooling_param { <pooling_param0> }
   	*  				| EPSILON
	*/
	/*std::cout << "pooling_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "pooling_param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& poolingParam0(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::poolingParam0(Layer* aLayer)
{
	/**
	* Production:	<pooling_param0> --> <pool> <pad> <pad_h> <pad_w> <kernel_size> <kernel_h> <kernel_w> <stride> 
	*					<stride_h> <stride_w> <engine> <global_pooling> <round_mode>
	*/
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "pool")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "pad")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "pad_h")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "pad_w")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "kernel_size")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "kernel_h")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "kernel_w")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "stride")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "stride_h")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "stride_w")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "engine")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "global_pooling")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "round_mode")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "pool")
			&& !pool(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "pad")
			&& !pad(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "pad_h")
			&& !pad_h(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "pad_w")
			&& !pad_w(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "kernel_size")
			&& !kernel_size(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "kernel_h")
			&& !kernel_h(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "kernel_w")
			&& !kernel_w(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "stride")
			&& !stride(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "stride_h")
			&& !stride_h(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "stride_w")
			&& !stride_w(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "engine")
			&& !engine(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "global_pooling")
			&& !global_pooling(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "round_mode")
			&& !round_mode(aLayer))
		{
			return false;
		}
	}
	return true;
}

bool Grammar::concatParam(Layer* aLayer)
{
	/**
	* Production:	<concat_param> --> concat_param { <concat_param0> }
   	*  				| EPSILON
	*/
	/*std::cout << "concat_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "concat_param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& concatParam0(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::concatParam0(Layer* aLayer)
{
	/**
	* Production:	<concat_param0> --> <axis> <concat_dim>
	*				| <concat_dim> <axis>
	*/
	if (parse->curToken())
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "axis"))
		{
			if(axis(aLayer))
			{
				if (!strcmp(parse->curToken()->getTokenName().c_str(), "concat_dim"))
				{
					return concat_dim(aLayer);
				}
			}
		}
		else if(!strcmp(parse->curToken()->getTokenName().c_str(), "concat_dim"))
		{
			if(concat_dim(aLayer))
			{
				if (!strcmp(parse->curToken()->getTokenName().c_str(), "axis"))
				{
					return axis(aLayer);
				}
				return true;
			}
		}
	}
	return false;
}

bool Grammar::dropoutParam(Layer* aLayer)
{
	/**
	* Production:	<dropout_param> --> dropout_param { <dropout_param0> }
   	*  				| EPSILON
	*/
	/*std::cout << "dropout_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "dropout_param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& dropoutParam0(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::dropoutParam0(Layer* aLayer)
{
	/**
	* Production:	<dropout_param0> --> <dropout_ratio>
	*/
	if (parse->curToken())
	{
		if (!strcmp(parse->curToken()->getTokenName().c_str(), "dropout_ratio"))
		{
			return dropout_ratio(aLayer);
		}
	}
	return true;
}

bool Grammar::reshapeParam(Layer* aLayer)
{
	/**
	* Production:	<reshape_param> --> reshape_param { <reshape_param0> }
   	*  				| EPSILON
	*/
	/*std::cout << "reshape_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "reshape_param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& reshapeParam0(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::reshapeParam0(Layer* aLayer)
{
	/**
	* Production:	<reshape_param0> --> <shape> <axis> <num_axes>
	*/
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "shape")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "axis")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "num_axes")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "shape")
			&& !shape(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "axis")
			&& !axis(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "num_axes")
			&& !num_axes(aLayer))
		{
			return false;
		}
	}
	return true;
}

bool Grammar::softmaxParam(Layer* aLayer)
{
	/**
	* Production:	<softmax_param> --> softmax_param { <softmax_param0> }
   	*  				| EPSILON
	*/
	/*std::cout << "softmax_param" << std::endl;*/
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "softmax_param")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
		&& parse->nextToken()
		&& softmaxParam0(aLayer)
		&& parse->curToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::softmaxParam0(Layer* aLayer)
{
	/**
	* Production:	<softmax_param0> --> <engine> <axis>
	*				| <axis> <engine>
	*/
	if (parse->curToken())
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "engine"))
		{
			if(engine(aLayer))
			{
				if (!strcmp(parse->curToken()->getTokenName().c_str(), "axis"))
				{
					return axis(aLayer);
				}
				return true;
			}
		}
		else if(!strcmp(parse->curToken()->getTokenName().c_str(), "axis"))
		{
			if(axis(aLayer))
			{
				if (!strcmp(parse->curToken()->getTokenName().c_str(), "engine"))
				{
					return engine(aLayer);
				}
				return true;
			}
		}
	}
	return false;
}

bool Grammar::num_output(Layer* aLayer)
{
	//Production: <num_output> --> num_output: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "num_output")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("num_output", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::bias_term(Layer* aLayer)
{
	//Production: <bias_term> --> bias_term: BOOL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "bias_term")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_BOOL)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("bias_term", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::pad(Layer* aLayer)
{
	//Production: <pad> --> <pad0> <pad>
	if(pad0(aLayer))
	{
		return pad(aLayer);
	}
	return true;
}

bool Grammar::pad0(Layer* aLayer)
{
	//Production: <pad0> --> pad: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "pad")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("pad", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::kernel_size(Layer* aLayer)
{
	//Production: <kernel_size> --> <kernel_size0> <kernel_size>
	if(kernel_size0(aLayer))
	{
		return kernel_size(aLayer);
	}
	return true;
}

bool Grammar::kernel_size0(Layer* aLayer)
{
	//Production: <kernel_size0 --> kernel_size: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "kernel_size")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("kernel_size", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::stride(Layer* aLayer)
{
	//Production: <stride> --> <stride0> <stride>
	if(stride0(aLayer))
	{
		return stride(aLayer);
	}
	return true;
}

bool Grammar::stride0(Layer* aLayer)
{
	//Production: <stride0> --> stride: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "stride")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("stride", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::dilation(Layer* aLayer)
{
	//Production: <dilation> --> <dilation0> <dilation>
	if(dilation0(aLayer))
	{
		return dilation(aLayer);
	}
	return true;
}

bool Grammar::dilation0(Layer* aLayer)
{
	//Production: <dilation> --> dilation: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "dilation")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("dilation", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}
	return false;
}

bool Grammar::pad_h(Layer* aLayer)
{
	//Production: <pad_h> --> pad_h: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "pad_h")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("pad_h", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::pad_w(Layer* aLayer)
{
	//Production: <pad_w> --> pad_w: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "pad_w")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("pad_w", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::kernel_h(Layer* aLayer)
{
	//Production: <kernel_h> --> kernel_h: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "kernel_h")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("kernel_h", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::kernel_w(Layer* aLayer)
{
	//Production: <kernel_w> --> kernel_w: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "kernel_w")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("kernel_w", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}
	return false;
}

bool Grammar::global_pooling(Layer* aLayer)
{
	//Production: <global_pooling> --> global_pooling : BOOL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "global_pooling")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_BOOL)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("global_pooling", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::round_mode(Layer* aLayer)
{
	//Production: <round_mode> --> round_mode : ROUNDMODE
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "round_mode")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_ROUNDMODE)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("round_mode", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::stride_h(Layer* aLayer)
{
	//Production: <stride_h> --> stride_h: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "stride_h")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("stride_h", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::stride_w(Layer* aLayer)
{
	//Production: <stride_w> --> stride_w: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "stride_w")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("stride_w", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::group(Layer* aLayer)
{
	//Production: <group> --> group: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "group")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("group", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::weight_filler(Layer* aLayer)
{
	//Production: <weight_filler> --> weight_filler { <FillerParameter> }
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "weight_filler")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
	&& parse->nextToken()
	&& FillerParameter(aLayer)
	&& parse->curToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		return true;
	}
	return false;
}

bool Grammar::FillerParameter(Layer* aLayer)
{
	//Production: <FillerParameter> --> <type> <value> <min> <max> <mean> <std> <sparse> <variance_norm>
	while(parse->curToken() && 
		(!strcmp(parse->curToken()->getTokenName().c_str(), "type")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "value")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "min")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "max")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "mean")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "std")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "sparse")
		|| !strcmp(parse->curToken()->getTokenName().c_str(), "variance_norm")))
	{
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "type")
			&& !type(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "min")
			&& !min(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "max")
			&& !max(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "pad_w")
			&& !pad_w(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "mean")
			&& !mean(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "std")
			&& !std(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "sparse")
			&& !sparse(aLayer))
		{
			return false;
		}
		if(!strcmp(parse->curToken()->getTokenName().c_str(), "stride")
			&& !variance_norm(aLayer))
		{
			return false;
		}
	}
	return true;
}

bool Grammar::filler(Layer* aLayer)
{
	//Production: <filler> --> filler { <FillerParameter> }
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "filler")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
	&& parse->nextToken()
	&& FillerParameter(aLayer)
	&& parse->curToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		return true;
	}
	return false;
}

bool Grammar::bias_filler(Layer* aLayer)
{
	//Production: <bias_filler> --> bias_filler { <FillerParameter> }
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "bias_filler")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
	&& parse->nextToken()
	&& FillerParameter(aLayer)
	&& parse->curToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		return true;
	}
	return false;
}

bool Grammar::engine(Layer* aLayer)
{
	//Production: <engine> --> engine: ENGINE
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "engine")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_ENGINE)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("engine", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::axis(Layer* aLayer)
{
	//Production: <axis> --> axis: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "axis")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("axis", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::force_nd_im2col(Layer* aLayer)
{
	//Production: <force_nd_im2col> --> force_nd_im2col: BOOL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "force_nd_im2col")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_BOOL)
	{		
		aLayer->fields.push_back(std::pair<std::string, std::string>("force_nd_im2col", parse->curToken()->getTokenName().c_str()));		
		parse->nextToken();		
		return true;	
	}	
	return false;
}

bool Grammar::value(Layer* aLayer)
{
	//Production: <value> --> value: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "value")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("value", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::min(Layer* aLayer)
{
	//Production: <min> --> min: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "min")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("min", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::max(Layer* aLayer)
{
	//Production: <max> --> max: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "max")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("max", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::mean(Layer* aLayer)
{
	//Production: <mean> --> mean: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "mean")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("mean", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::std(Layer* aLayer)
{
	//Production: <std> --> std: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "std")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("std", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::sparse(Layer* aLayer)
{
	//Production: <sparse> --> sparse: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "sparse")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("sparse", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::variance_norm(Layer* aLayer)
{
	//Production: <variance_norm> --> variance_norm: VARIANCENORM
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "variance_norm")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_VARIANCENORM)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("variance_norm", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::use_global_stats(Layer* aLayer)
{
	//Production: <use_global_stats> --> use_global_stats: BOOL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "use_global_stats")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_BOOL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("use_global_stats", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::moving_average_fraction(Layer* aLayer)
{
	//Production: <moving_average_fraction> --> moving_average_fraction: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "moving_average_fraction")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("moving_average_fraction", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::eps(Layer* aLayer)
{
	//Production: <eps> --> eps: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "eps")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("eps", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::num_axes(Layer* aLayer)
{
	//Production: <num_axes> --> num_axes: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "num_axes")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("num_axes", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::negative_slope(Layer* aLayer)
{
	//Production: <negative_slope> --> negative_slope: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "negative_slope")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("negative_slope", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::concat_dim(Layer* aLayer)
{
	//Production: <concat_dim> --> concat_dim: INT
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "concat_dim")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_INT)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("concat_dim", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::dropout_ratio(Layer* aLayer)
{
	//Production: <dropout_ratio> --> dropout_ratio: DECIMAL
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "dropout_ratio")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
	&& parse->nextToken()
	&& parse->curToken()->getID() == Token::IDTYPE_DECIMAL)
	{
		aLayer->fields.push_back(std::pair<std::string, std::string>("dropout_ratio", parse->curToken()->getTokenName().c_str()));
		parse->nextToken();
		return true;
	}
	return false;
}

bool Grammar::shape(Layer* aLayer)
{
	//Production: <shape> --> shape { <BlobShape> }
	if (parse->curToken()
	&& !strcmp(parse->curToken()->getTokenName().c_str(), "shape")
	&& parse->nextToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE
	&& parse->nextToken()
	&& BlobShape(aLayer)
	&& parse->curToken()
	&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		return true;
	}
	return false;
}

bool Grammar::BlobShape(Layer* aLayer)
{
	/** Production: <BlobShape> --> <dim> <BlobShape>
	 * 								| EPSILON
	 */
	 if (dim(aLayer))
	 {
		 return BlobShape(aLayer);
	 }
	 return true;
}

bool Grammar::pool(Layer* aLayer)
{
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "pool")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_POOLMETHOD)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("pool", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

bool Grammar::dim(Layer* aLayer)
{
	/** Production:	<dim> --> dim : INT
	 */
	if (parse->curToken()
		&& !strcmp(parse->curToken()->getTokenName().c_str(), "dim")
		&& parse->nextToken()
		&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON
		&& parse->nextToken()
		&& parse->curToken()->getID() == Token::IDTYPE_INT)
		{
			aLayer->fields.push_back(std::pair<std::string, std::string>("dim", parse->curToken()->getTokenName().c_str()));
			parse->nextToken();
			return true;
		}
		return false;
}

void Grammar::instantiateParser(Parser* newParser)
{
	parse = newParser;
}

