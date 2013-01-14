#include "WeakLearner.h"
#include "Samples.cpp"
#include "WeakClassifier.h"
#include "Utils.h"
#include <limits>
#ifdef AB_DEBUG
#include <iostream>
#endif

float WeakLearner::INFINITESIMAL = pow(-6);

WeakLearner::WeakLearner(): feature(std::numeric_limits<float>::min()), threshold(std::numeric_limits<float>::min()), 
	alpha(std::numeric_limits<float>::min()) {
}

/* 
 * We assume here the samples to train can be divided into two categories, +1, and -1.
 * We try to classify the samples based on features one by one, and when dividing the samples by some feature, we try 'stepNum' threshold values.
 * And find the classification that gains lowest prediction error rate, record the related information: the feature based on, and threshold value.
 */
float WeakLearner::train(const Samples<float>& trainingSamples, const float* categories, const std::pair<float, float>* featureRanges, 
	const int stepNum, float* weights) {
	int sampleNum = trainingSamples.getSampleNum();
	float minErrorRate = 1.0;
	float* bestPredictedCategories = new float[sampleNum];	
	float* predictedCategories = new float[sampleNum];

	int featureNum = trainingSamples.getFeatureNum();
	for(int j = 0; j < featureNum; ++j) {
		float featureJMin = featureRanges[j].first;
		float featureJMax = featureRanges[j].second;
		float stepSize = (featureJMax - featureJMin) / stepNum;
		for(int step = -1; step <= stepNum; ++step) {
			float threshold = featureJMin + step * stepSize;
			for(int i = 0; i < sampleNum; ++i)
				predictedCategories[i] = WeakClassifier::classify(trainingSamples.getSample(i), j, threshold);	// categories: -1, 1
#ifdef AB_DEBUG
			std::cout<<"feature = "<<j<<", threshold = "<<threshold<<std::endl;
			for(int i = 0; i < sampleNum; ++i)
				std::cout<<predictedCategories[i]<<" ";
			std::cout<<std::endl;
#endif
			float errorRate = 0.0;
			for(int i = 0; i < sampleNum; ++i)
				if(predictedCategories[i] != categories[i])
					errorRate += weights[i];
#ifdef AB_DEBUG
			std::cout<<"errorRate = "<<errorRate<<std::endl;
#endif
			if(minErrorRate > errorRate) {
				for(int i = 0; i < sampleNum; ++i)
					bestPredictedCategories[i] = predictedCategories[i];
				this->feature = j;
				this->threshold = threshold;
				this->alpha = 1.0/2*ln((1-errorRate) / max(errorRate, INFINITESIMAL));	// 1/2: 1/(1 - (-1))
#ifdef AB_DEBUG
				//std::cout<<"feature = "<<this->feature<<", threshold = "<<this->threshold<<", alpha = "<<this->alpha<<std::endl;
#endif

				minErrorRate = errorRate;
			}
		}
	}
	delete[] predictedCategories;

	// update 'weights' using the best weak classifier which classifies samples based on feature indexed by 'feature'
	float z = 0.0;
    for(int i = 0; i < sampleNum; ++i) {
        /*
		 * Since predicatedCategories[i] and categories[i] are either +1 or -1.
		 * So multiplication of them will be 1 if equal, -1 otherwise.  
		 * Question: What change should be made here to extend to more than two categories case?
		 */
        float factor = bestPredictedCategories[i] * categories[i];
        weights[i] *= pow(-this->alpha * factor);
		z += weights[i];
    }
	// normalize weights to make it a distribution, that is, sum of weights[i] amouts to 1.0. Very important!!!
	for(int i = 0; i < sampleNum; ++i)
		weights[i] /= z;
	delete[] bestPredictedCategories;

	return minErrorRate;
}

WeakLearner::~WeakLearner() {
}
