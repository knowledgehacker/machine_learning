/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "InputDataSet.h"
#include <iostream>

int main(int argc, char** argv) {
	InputDataSet ids("trainingDigits");

	int samplesCount = 0; 
	while(ids.hasNext() && (samplesCount++ < 3)) {
		std::vector<std::bitset<1024> > samples = ids.next();
		int sampleDim = InputDataSet::SAMPLE_DIM;

		int sampleCount = 0;
		int sampleNum = samples.size();
		for(int i = 0; (i < sampleNum) && (sampleCount < 3); ++i, ++sampleCount) {
			std::bitset<1024> sample = samples[i];
			for(int j = 0; j < sampleDim; ++j) {
				int startIdx = j*sampleDim;
				for(int k = 0; k < sampleDim; ++k)
					std::cout<<sample[startIdx+k];
				std::cout<<std::endl;
			}
		}
	}

	return 0;
}
