/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _K_NEAREST_NEIGHBOURS_H_
#define _K_NEAREST_NEIGHBOURS_H_

#include "InputDataSet.h"

/*
 * Design of InputDataSet has a drawback that iterating it will change the state of InputDataSet.
 * Thus here we can only pass InputDataSet objects by non-const reference.
 */
class KNearestNeighbours {
private:
	InputDataSet& trainingSet;
	InputDataSet& testSet;
	int kNN;

	int totalTestSample;
	int errorTestSample;

public:
	KNearestNeighbours(InputDataSet& training, InputDataSet& test, int k);
	bool KNN(int testDigit, const std::bitset<1024>& testSample);
	float getErrorRate() const;	
	~KNearestNeighbours();

private:
	float distance(const std::bitset<1024>& sample1, const std::bitset<1024>& sample2)const;
};

#endif
