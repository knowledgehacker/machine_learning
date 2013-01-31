/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "FloatDataReader.h"
#include "Samples.cpp"
#include "AdaptiveBoost.h"
#include "Timing.h"
#include <iostream>

int main(int argc, char** argv) {
	int64 start = getTimeMs64();
	Samples<float> trainingSamples("data/points.txt", FloatDataReader());
	int sampleNum = trainingSamples.getSampleNum();
	int featureNum = trainingSamples.getFeatureNum();
	for(int i = 0; i < sampleNum; ++i) {
		const float* sample = trainingSamples.getSample(i);
		for(int j = 0; j < featureNum; ++j)
			std::cout<<sample[j]<<" ";
		std::cout<<sample[featureNum]<<std::endl;
	}
	AdaptiveBoost adaptiveBoost(trainingSamples, 0.01);
	float p0_0[] = {0.0, 0.0};
	std::cout<<"p0_0 = "<<adaptiveBoost.classify(p0_0)<<std::endl;
	float p18_08[] = {1.8, 0.8};
	std::cout<<"p18_08 = "<<adaptiveBoost.classify(p18_08)<<std::endl;
	float p5_5[] = {5.0, 5.0};
	std::cout<<"p5_5 = "<<adaptiveBoost.classify(p5_5)<<std::endl;
	std::cout<<"Execution time: "<<getTimeMs64() - start<<"ms"<<std::endl;

/*
	FloatDataReader fdr;
	std::string trainingFile("data/horseColicTraining.txt");
	Samples<float> trainingSamples(trainingFile, fdr);
	AdaptiveBoost adaptiveBoost(trainingSamples, 0.01);

	std::string testFile("data/horseColicTest.txt");
	Samples<float> testSamples(testFile, fdr);
	int testSampleNum = testSamples.getSampleNum();
	for(int i = 0; i < testSampleNum; ++i)
		adaptiveBoost.classify(testSamples.getSample(i));
*/

	return 0;
}
