/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "StringDataReader.h"
#include "Samples.cpp"
#include "DecisionTree.h"
#include "Timing.h"
#include <iostream>

int main(int argc, char** argv) {
	std::string dataFile = "data/species.txt";
	Samples<std::string> trainingSamples(dataFile, StringDataReader());

	int64 start = getTimeMs64();
	DecisionTree decisionTree(trainingSamples);
	//for(int i = 0; i < 1000; ++i) {
		decisionTree.breadthFirstTraverse();
	//}
    std::cout<<"execution time: "<<getTimeMs64() - start<<"ms"<<std::endl;

	
	return 0;
}
