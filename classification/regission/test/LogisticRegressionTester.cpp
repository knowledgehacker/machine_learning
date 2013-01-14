#include "FloatDataReader.h"
#include "Samples.cpp"
#include "Sigmoid.h"
#include "LogisticRegression.h"
#include <iostream>

int main(int argc, char** argv) {
	std::string trainingFile = "data/horseColicTraining.txt";
	Samples<float> trainingSamples(trainingFile, FloatDataReader());
	float alpha = 0.01;
	Sigmoid sigmoid;
	float threshold = 0.5;
	LogisticRegression lr(trainingSamples, alpha, sigmoid, threshold);
	
/*
	std::string testFile = "data/horseColicTest.txt";
	Samples<float> testSamples(testFile, lrConverter);
	int featureNum = testSamples.getFeatureNum();
	int testSampleNum = testSamples.getSampleNum();
	for(int i = 0; i < testSampleNum; ++i) {
		const float* testSample = testSamples.getSample(i);
		int category = lr.classify(testSample);
		std::cout<<"sample "<<i<<": "<<"predicted = "<<category<<", expected = "<<testSample[featureNum]<<std::endl;
	}
*/

	return 0;
}
