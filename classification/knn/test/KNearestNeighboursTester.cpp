/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "KNearestNeighbours.h"
#include "Timing.h"
#include <iostream>

int main(int argc, char** argv) {
	std::string trainingDir = "trainingDigits";
	std::string testDir = "testDigits";
	//int k = 21;
	int k = 3;
	int64 start = getTimeMs64();
	InputDataSet trainingSet(trainingDir), testSet(testDir);
	std::cout<<"Execution time of creating training and test sets: "<<getTimeMs64() - start<<"ms"<<std::endl;

	start = getTimeMs64();
	KNearestNeighbours kNearestNeighbours(trainingSet, testSet, k);
	double errorRate = kNearestNeighbours.getErrorRate();
	std::cout<<"errorRate = "<<errorRate<<std::endl;
	std::cout<<"Execution time of K-Nearest Neighbours algorithm: "<<getTimeMs64() - start<<"ms"<<std::endl;

	return 0;
}
