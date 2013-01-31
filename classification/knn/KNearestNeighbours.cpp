/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "KNearestNeighbours.h"
#include "NMinHeap.cpp"
#include <cmath>
#include <iostream>

KNearestNeighbours::KNearestNeighbours(InputDataSet& training, InputDataSet& test, int k): 
	trainingSet(training), testSet(test), kNN(k), totalTestSample(0), errorTestSample(0) {
	int testDigit = 0;
	while(testSet.hasNext()) {
		std::vector<std::bitset<1024> >& testSamples = testSet.next();
		int testSampleNum = testSamples.size();
		for(int i = 0; i < testSampleNum; ++i) {
			//std::cout<<"test: "<<testDigit<<"_"<<i<<std::endl;
			if(!KNN(testDigit, testSamples[i]))
				++errorTestSample;
		}
		totalTestSample += testSampleNum;

		++testDigit;
	}
}

bool KNearestNeighbours::KNN(int testDigit, const std::bitset<1024>& testSample) {
	NMinHeap<double, int> kNNSet(kNN);

	int trainingDigit = 0;
	while(trainingSet.hasNext()) {
		//std::cout<<"training: "<<trainingDigit<<"_: ";
		std::vector<std::bitset<1024> >& trainingSamples = trainingSet.next();
		int trainingSampleNum = trainingSamples.size();
		for(int i = 0; i < trainingSampleNum; ++i) {
			//std::cout<<i<<", ";
			kNNSet.insert(distance(testSample, trainingSamples[i]), trainingDigit);
		}
		++trainingDigit;
		//std::cout<<std::endl;
	}

	int count[InputDataSet::DIGIT_NUM] = {0};
	int size = kNNSet.size();
	for(int i = 0; i < size; ++i)
		++count[kNNSet[i].second];

	int digit = 0;
	int times = count[0];
	for(int i = 1; i < InputDataSet::DIGIT_NUM; ++i)
		if(count[i] > times) {
			digit = i;
			times = count[i];
		}

	if(digit == testDigit) {
		//std::cout<<"matched: testDigit = "<<testDigit<<std::endl;
		return true;
	}

	//std::cout<<"dismatched: testDigit = "<<testDigit<<", digit = "<<digit<<std::endl;
	return false;
}

float KNearestNeighbours::getErrorRate()const {
	std::cout<<"errorTestSample = "<<errorTestSample<<", totalTestSample = "
		<<totalTestSample<<std::endl;
	return static_cast<float>(errorTestSample) / static_cast<float>(totalTestSample);
}

/*
 * Performance boosts greatly when I optimized the code snippet below, 
 * the execution time reduces from 3897ms to 616ms, amazing!
 * int squareSum = 0;
 * for(int i = 0; i < 1024; ++i) {
 *     int diff = sample1[i] ^ sample2[i];
 *	   squareSum += diff;	// as to a value can be either 0 or 1, value == value*value 
 * }
 *
 * =>
 *
 * int squareSum = (sample1 ^ sample2).count();
 */
float KNearestNeighbours::distance(const std::bitset<1024>& sample1, 
	const std::bitset<1024>& sample2)const {
	int squareSum = (sample1 ^ sample2).count();

	return sqrt(static_cast<float>(squareSum));
}

KNearestNeighbours::~KNearestNeighbours() {
}

