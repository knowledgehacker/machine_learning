#include "DecisionTree.h"
#include "Timing.h"
#include <iostream>

int main(int argc, char** argv) {
	InputDataSet inputDataSet("data/species.txt");

	int64 start = getTimeMs64();
	DecisionTree decisionTree(inputDataSet);
	for(int i = 0; i < 1000; ++i) {
		decisionTree.breadthFirstTraverse();
	}
    std::cout<<"execution time: "<<getTimeMs64() - start<<"ms"<<std::endl;

	
	return 0;
}
