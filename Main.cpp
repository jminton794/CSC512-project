#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include "Scanner.h"
#include "Parser.h"
#include "Layer.h"

void genOutFName(char *input, std::string &output);
void printConv2d(Layer* convLayer, Layer* nextLayer, bool isConcat, std::string branchString, std::queue<Layer*> conv2dLayers, std::ofstream& outFile);
void printConcat(Layer* concatLayer, std::queue<Layer*> conv2dLayers, std::queue<Layer*> concatLayers, std::ofstream& outFile);
void printPooling(Layer* poolLayer, Layer* nextLayer, bool isConcat, std::string branchString, std::queue<Layer*> conv2dLayers, std::ofstream& outFile);
void printLogits(std::queue<Layer*> logitLayers, std::ofstream& outFile);
bool search(std::vector< std::string> bigString, std::string vectorString);
std::string dualSearch(std::vector< std::pair<std::string, std::string> > bigString, std::string vectorString);
bool compTopBottom(Layer* firstLayer, Layer* secondLayer);
bool searchQueue(std::queue<Layer*> aQueue, std::string);
bool searchBoth(std::vector< std::string> firstString, std::vector< std::string> secondString);

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cout << "Invalid number of arguments." << std::endl << "Format:\t./Main <InputFileName>" << std::endl;
		return EXIT_FAILURE;
	}

	Scanner scan(argv[1]);

	std::string outFileName;
	genOutFName(argv[1], outFileName);
	std::ofstream outFile(outFileName.c_str(), std::ifstream::out);
	
	Grammar *cGrammar = new Grammar(&outFile);
	SymbolTable *newSymbolTable = new SymbolTable();
	Parser parse(cGrammar, newSymbolTable);

	while (scan.hasNextToken())
	{
		Token *t = scan.getNextToken();
		//std::cout << t->getTokenName().c_str() << std::endl;
		if (t->getID() == Token::IDTYPE_INVALID)
		{
			delete t;
			std::cout << "Error" << std::endl;
			return EXIT_FAILURE;
		}
		parse.addToken(t);
	}

	if (!parse.parseProgram())
	{
		std::cout << "Error" << std::endl;
		return EXIT_FAILURE;
	}
	
	//CodeGen* simpleGen = new CodeGen(newSymbolTable, &outFile);
	//simpleGen->printSymTab();
}

void genOutFName(char *input, std::string &output)
{
	bool printed = false;
	while (*input != '\0')
	{
		if (*input == '.' && !printed)
		{
			output.push_back('_');
			output.push_back('s');
			output.push_back('i');
			output.push_back('m');
			output.push_back('p');
			output.push_back('l');
			output.push_back('e');
			output.push_back('.');
			output.push_back('p');
			output.push_back('y');
			printed = true;
			break;
			/*std::std::cout << " I [" << input << "] output [" << output << "]\n";*/
		}
		else
		{
			output.push_back(*input);
			/*std::std::cout << " I [" << input << "] output [" << output << "]\n";*/
		}
		input++;
	}
}

std::ofstream& operator<<(std::ofstream& outFile, SymbolTable& printSymTab)
{
	outFile << "from __future__ import absolute_import\nfrom __future__ import division\nfrom __future__ import print_function\n\nimport tensorflow as tf\n\nslim = tf.contrib.slim\n\n";
	outFile << std::endl << "def " << printSymTab.scope << "(inputs," << std::endl;
	outFile << std::endl << "\t\t\t\t\tnum_classes=" << printSymTab.numClasses << "," << std::endl;
	outFile << std::endl << "\t\t\t\t\tis_training=True," << std::endl,
	outFile << std::endl << "\t\t\t\t\treuse=None," << std::endl;
	outFile << std::endl << "\t\t\t\t\tscope='" << printSymTab.scope << "'):" << std::endl;
	
	outFile << std::endl << std::endl;
	
	outFile << "with tf.variable_scope(scope, \"Model\", reuse=reuse):" << std::endl;
	outFile << "  with slim.arg_scope(default_arg_scope(is_training)):" << std::endl;
	outFile << std::endl;
	outFile << "    end_points = {}";
	outFile << std::endl;
	std::queue<Layer*> conv2dLayers;
	std::queue<Layer*> concatLayers;
	std::queue<Layer*> logitLayers;
	
	while (!(printSymTab.layerQueue.empty()))
	{
		Layer* hold = printSymTab.layerQueue.front();
		printSymTab.layerQueue.pop();
		if (!hold->layerType.compare("Convolution"))
		{
			conv2dLayers.push(hold);
			if (!printSymTab.layerQueue.front()->layerType.compare("BatchNorm"))
			{
				hold = printSymTab.layerQueue.front();
				conv2dLayers.push(hold);
				if (!printSymTab.layerQueue.front()->layerType.compare("Scale"))
				{	
					hold = printSymTab.layerQueue.front();
					printSymTab.layerQueue.pop();
					conv2dLayers.push(hold);
					if (!printSymTab.layerQueue.front()->layerType.compare("ReLU"))
					{
						hold = printSymTab.layerQueue.front();
						printSymTab.layerQueue.pop();
						conv2dLayers.push(hold);
						Layer* finalConv = conv2dLayers.front();
						conv2dLayers.pop();
						Layer* conv2 = conv2dLayers.front();
						conv2dLayers.pop();
						for (std::vector< std::pair<std::string, std::string> >::iterator it = conv2->fields.begin(); it != conv2->fields.end(); ++it)
						{
							finalConv->fields.push_back(std::pair<std::string, std::string>(it->first, it->second));
						}
						conv2 = conv2dLayers.front();
						conv2dLayers.pop();
						for (std::vector< std::pair<std::string, std::string> >::iterator it = conv2->fields.begin(); it != conv2->fields.end(); ++it)
						{
							finalConv->fields.push_back(std::pair<std::string, std::string>(it->first, it->second));
						}
						conv2 = conv2dLayers.front();
						conv2dLayers.pop();
						for (std::vector< std::pair<std::string, std::string> >::iterator it = conv2->fields.begin(); it != conv2->fields.end(); ++it)
						{
							finalConv->fields.push_back(std::pair<std::string, std::string>(it->first, it->second));
						}
						delete conv2;
						conv2dLayers.push(finalConv);
					}
					else
					{
						std::cout << "Error: improper layer order" << std::endl;
						return outFile;
					}
				}
				else
				{
					std::cout << "Error: improper layer order" << std::endl;
					return outFile;
				}
			}
			else
			{
				continue;
			}
		}
		else if (!hold->layerType.compare("Pooling"))
		{
			conv2dLayers.push(hold);
		}
		else if (!hold->layerType.compare("Concat"))
		{
			printConcat(hold, conv2dLayers, concatLayers, outFile);
		}
		else if (!hold->layerType.compare("Dropout"))
		{
			if (!conv2dLayers.empty())
			{
				while (!conv2dLayers.empty())
				{
					if (!conv2dLayers.front()->layerType.compare("Convolution"))
					{
						Layer* holdConv = conv2dLayers.front();
						conv2dLayers.pop();
						printConv2d(holdConv, conv2dLayers.front(), false, "null", conv2dLayers, outFile);
					}
					else
					{
						Layer* holdPool = conv2dLayers.front();
						conv2dLayers.pop();
						printPooling(holdPool, conv2dLayers.front(), false, "null", conv2dLayers, outFile);
					}
				}
			}
			logitLayers.push(hold);
			if (!printSymTab.layerQueue.front()->layerType.compare("Convolution"))
			{
				hold = printSymTab.layerQueue.front();
				printSymTab.layerQueue.pop();
				logitLayers.push(hold);
				if (!printSymTab.layerQueue.front()->layerType.compare("Reshape"))
				{
					hold = printSymTab.layerQueue.front();
					printSymTab.layerQueue.pop();
					logitLayers.push(hold);
				}
				
			}
		}
		else if (!hold->layerType.compare("Softmax"))
		{
			std::string top = hold->top;
			std::string bottom = hold->bottoms.front();
			std::string axis = dualSearch(hold->fields, "axis");
			outFile << "      end_points['" << top << "'] = slim.softmax(" << bottom << ", ";
			if (axis.compare("null"))
			{
				outFile << "axis=" << axis << ", ";
			}
			outFile << "scope='" << hold->name << "')" << std::endl;
		}
		else
		{
			std::cout << "Error: improper layer order" << std::endl;
			return outFile;
		}
	}
	outFile << " return logits, end_points" << std::endl << std::endl << std::endl;
	int shape = 0;
	int holdDim = 0;
	std::stringstream s;
	for (std::vector< std::string>::iterator it = printSymTab.dims.begin(); it!= printSymTab.dims.end(); ++it)
	{
		s << *it;
		s >> holdDim;
		if (holdDim > shape)
		{
			shape = holdDim;
		}
	}
	outFile << printSymTab.scope << ".default_image_size=" << shape << std::endl << std::endl << std::endl;
	
	//Hardcoded text
	outFile << std::endl << "def default_arg_scope(is_training=True, " << std::endl
                        << "\t \t \t \t \t weight_decay=0.00004," << std::endl
                        << "\t \t \t \t \t use_batch_norm=True," << std::endl
                        << "\t \t \t \t \t batch_norm_decay=0.9997," << std::endl
                        << "\t \t \t \t \t batch_norm_epsilon=0.001," << std::endl
                        << "\t \t \t \t \t batch_norm_updates_collections=tf.GraphKeys.UPDATE_OPS): " << std::endl;
	outFile << std::endl << "batch_norm_params = {" << std::endl
						<< "\t # Decay for the moving averages." << std::endl
						<< "\t 'decay': batch_norm_decay," << std::endl
						<< "\t # epsilon to prevent 0s in variance." << std::endl
						<< "\t 'epsilon': batch_norm_epsilon," << std::endl
						<< "\t # collection containing update_ops." << std::endl
						<< "\t 'updates_collections': batch_norm_updates_collections," << std::endl
						<< "\t # use fused batch norm if possible." << std::endl
						<< "\t 'fused': None," << std::endl
						<< "}" << std::endl;
	outFile << "if use_batch_norm:" << std::endl
			<< "\t normalizer_fn = slim.batch_norm" << std::endl
			<< "\t normalizer_params = batch_norm_params" << std::endl
			<< "else:" << std::endl
			<< "\t normalizer_fn = None" << std::endl
			<< "\t normalizer_params = {}" << std::endl;
	outFile << std::endl << "# Set training state " << std::endl
						<< "with slim.arg_scope([slim.batch_norm, slim.dropout]," << std::endl
						<< "\t \t \t \t \t is_training=is_training):" << std::endl
						<< "  # Set weight_decay for weights in Conv and FC layers." << std::endl
						<< " with slim.arg_scope([slim.conv2d, slim.fully_connected]," << std::endl
						<< " \t \t \t \t \t weights_regularizer=slim.l2_regularizer(weight_decay)):"
						<< "  # Set batch norm " << std::endl
						<< "  with slim.arg_scope(" << std::endl
						<< " \t \t [slim.conv2d]," << std::endl
						<< " \t \t normalizer_fn=normalizer_fn," << std::endl
						<< " \t \t normalizer_params=normalizer_params):" << std::endl
						<< " \t \t # Set default padding and stride" << std::endl
						<< " \t \t  with slim.arg_scope([slim.conv2d, slim.max_pool2d]," << std::endl
						<< " \t \t \t \t stride=1, padding='SAME') as sc:" << std::endl
						<< " \t \t \t return sc" << std::endl;
	return outFile;
}

void printConv2d(Layer* convLayer, Layer* nextLayer, bool isConcat, std::string branchString, std::queue<Layer*> conv2dLayers, std::ofstream& outFile)
{
	std::string numOutput = dualSearch(convLayer->fields, "num_output");
	std::string kernelSize = dualSearch(convLayer->fields, "kernel_size");
	std::string kernelHeight = dualSearch(convLayer->fields, "kernel_h");
	std::string kernelWidth = dualSearch(convLayer->fields, "kernel_w");
	std::string stride = dualSearch(convLayer->fields, "num_output");
	
	if (!isConcat)
	{
		outFile << "    end_point = " << convLayer->top << std::endl;
		outFile << "    net = slim.conv2d(";
	}
	else
	{
		outFile << "    " << branchString << " = slim.conv2d(";
	}
	
	if (search(convLayer->bottoms, "inputs"))
	{
		outFile << "inputs, ";
	}
	else if (compTopBottom(convLayer, nextLayer))
	{
		outFile << branchString << ", ";
	}
	else
	{
		outFile << "net, ";
	}
	outFile << numOutput << ", [";
	if (kernelHeight.compare("null"))
	{
		outFile << kernelSize << ", " << kernelSize << "], ";
	}
	else
	{
		outFile << kernelWidth << ", " << kernelHeight << "], ";
	}
	if (stride.compare("null"))
	{
		outFile << "stride=" << stride << ", ";
	}
	if(isConcat)
	{
		outFile << "scope='" << convLayer->name << "')" << std::endl;
	}
	else
	{
		outFile << "scope=end_point)" << std::endl;
	}
	outFile << "end_points[end_point] = net" << std::endl << std::endl;
}

void printPooling(Layer* poolLayer, Layer* nextLayer, bool isConcat, std::string branchString, std::queue<Layer*> conv2dLayers, std::ofstream& outFile)
{
	outFile << "    end_point = '" << poolLayer->name << "'" << std::endl;
	std::string poolParamCheck = dualSearch(poolLayer->fields, "pool");
	std::string kernelSize = dualSearch(poolLayer->fields, "kernel_size");
	std::string kernelHeight = dualSearch(poolLayer->fields, "kernel_h");
	std::string kernelWidth = dualSearch(poolLayer->fields, "kernel_w");
	std::string stride = "1";
	
	if (!isConcat)
	{
		outFile << "    end_point = " << poolLayer->top << std::endl;
		outFile << "    net = slim.";
	}
	else
	{
		outFile << "    " << branchString << " = slim.";
	}
	
	if (!poolParamCheck.compare("AVE"))
	{
		outFile << "avg_pool2d(";
	}
	else
	{
		outFile << "max_pool2d(";
	}
	
		if (search(poolLayer->bottoms, "inputs"))
	{
		outFile << "inputs, [";
	}
	else if (compTopBottom(poolLayer, nextLayer))
	{
		outFile << branchString << ", [";
	}
	else
	{
		outFile << "net, [";
	}
	
	if (kernelHeight.compare("null"))
	{
		outFile << kernelSize << ", " << kernelSize << "], ";
	}
	else
	{
		outFile << kernelWidth << ", " << kernelHeight << "], ";
	}
	if (stride.compare("null"))
	{
		outFile << "stride=" << stride << ", ";
	}
	if(isConcat)
	{
		outFile << "scope='" << poolLayer->name << "')" << std::endl;
	}
	else
	{
		outFile << "scope=end_point)" << std::endl;
	}
	if (!isConcat)
	{
		outFile << "end_points[end_point] = net" << std::endl << std::endl;
	}
}

void printConcat(Layer* concatLayer, std::queue<Layer*> conv2dLayers, std::queue<Layer*> concatLayers, std::ofstream& outFile)
{
	outFile << "    end_point = '" << concatLayer->name << "'" << std::endl;
	outFile << "    with tf.variable_scope(end_point):" << std::endl;
	while(!conv2dLayers.empty())
	{
		Layer* hold = conv2dLayers.front();
		conv2dLayers.pop();
		if (search(concatLayer->bottoms, hold->top))
		{
			concatLayers.push(hold);
			break;
		}
		else if (searchBoth(hold->bottoms, concatLayer->bottoms))
		{
			concatLayers.push(hold);
			break;
		}
		else
		{
			if (!hold->layerType.compare("Convolution"))
			{
				printConv2d(hold, concatLayers.front(), false, "null", conv2dLayers, outFile);
			}
			else
				printPooling(hold, concatLayers.front(), false, "null", conv2dLayers, outFile);
		}
	}
	
	int i = 0;
	while(!concatLayers.empty())
	{
		Layer* holdConcat = concatLayers.front();
		concatLayers.pop();
		outFile << "      with tf.variablescope('Branch_" << i << "'):" << std::endl;
		std::string branchString = "branch_" + i;
		concatLayer->branchValues.push_back(branchString);
		outFile << "  ";
		printConv2d(holdConcat, concatLayers.front(), true, branchString, conv2dLayers, outFile);
		while (compTopBottom(holdConcat, concatLayers.front()))
		{
			holdConcat = concatLayers.front();
			concatLayers.pop();
			printConv2d(holdConcat, concatLayers.front(), true, branchString, conv2dLayers, outFile);
		}
		i++;
	}
	std::string axisNum = dualSearch(concatLayer->fields, "axis");
	outFile << "      net = tf.concat(" << std::endl;
	outFile << "          axis=" << axisNum << ", values = [";
	for (std::vector< std::string>::iterator it = concatLayer->branchValues.begin(); it != concatLayer->branchValues.end(); ++it)
	{
		if (it != concatLayer->branchValues.begin())
		{
			outFile << ", ";
		}
		outFile << *it;
	}
	outFile << "])" << std::endl;
	outFile << "    end_points[end_point] = net" << std::endl << std::endl << std::endl;
}

void printLogits(std::queue<Layer*> logitLayers, std::ofstream& outFile)
{
	outFile << "    end_point = 'Logits'\n    with tf.variable_scope(end_point):\n      net = slim.dropout(net, ";
	Layer* hold = logitLayers.front();
	logitLayers.pop();
	std::string dropoutRat = dualSearch(hold->fields, "dropout_ratio");
	std::stringstream s;
	s << dropoutRat;
	double trueDrop;
	s >> trueDrop;
	trueDrop = 1 - trueDrop;
	outFile << trueDrop << ", scope='" << hold->name << "')" << std::endl;
	
	hold = logitLayers.front();
	logitLayers.pop();
	
	std::string kernelSize = dualSearch(hold->fields, "kernel_size");
	std::string kernelHeight = dualSearch(hold->fields, "kernel_h");
	std::string kernelWidth = dualSearch(hold->fields, "kernel_w");
	std::string stride = dualSearch(hold->fields, "num_output");
	
	outFile << "      logits = slim.conv2d(net, num_classes, [";
	if (kernelHeight.compare("null"))
	{
		outFile << kernelSize << ", " << kernelSize << "], ";
	}
	else
	{
		outFile << kernelWidth << ", " << kernelHeight << "], ";
	}
	if (stride.compare("null"))
	{
		outFile << "stride=" << stride << ", ";
	}
	outFile << "activation_fn=None," << std::endl;
	outFile << "                           normalizer_fn=None, scope='" << hold->name << "')" << std::endl << std::endl;
	
	hold = logitLayers.front();
	logitLayers.pop();
	
	outFile << "      logits = tf.squeeze(logits, [1, 2], name='SpatialSqueeze')" << std::endl;
	outFile << "      end_points[end_point] = logits" << std::endl << std::endl;
}

bool search(std::vector< std::string> bigString, std::string vectorString)
{
	for(std::vector< std::string>::iterator it = bigString.begin(); it != bigString.end(); ++it)
	{
		std::string test = *it;
		if (!test.compare(vectorString))
		{
			return true;
		}
	}
	return false;
}

bool searchBoth(std::vector< std::string> firstString, std::vector< std::string> secondString)
{
	for(std::vector< std::string>::iterator it = firstString.begin(); it != firstString.end(); ++it)
	{
		if (search(secondString, *it))
		{
			return true;
		}
	}
}

std::string dualSearch(std::vector< std::pair<std::string, std::string> > bigString, std::string vectorString)
{
	for (std::vector< std::pair<std::string, std::string> >::iterator it = bigString.begin(); it != bigString.end(); ++it)
	{
		if (!it->first.compare(vectorString))
		{
			return it->second;
		}
	}
	return "null";
}

bool compTopBottom(Layer* firstLayer, Layer* secondLayer)
{
	for(std::vector< std::string>::iterator it = secondLayer->bottoms.begin(); it != secondLayer->bottoms.end(); ++it)
	{
		std::string test = *it;
		if (test.compare(firstLayer->top))
		{
			return true;
		}
	}
	return false;
}

bool searchQueue(std::queue<Layer*> aQueue, std::string topString)
{
	bool found = false;
	for (int i = 0; i < aQueue.size(); i++)
	{
		Layer* hold = aQueue.front();
		if (search(hold->bottoms, topString))
		{
			found = true;
		}
		aQueue.pop();
		aQueue.push(hold);
	}
	return found;
}


