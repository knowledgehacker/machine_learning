/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "AdaptiveBoost.h"
#include "Samples.cpp"
#include "WeakClassifier.h"
#include <utility>
#include <limits>
#ifdef AB_DEBUG
#include <iostream>
#endif

AdaptiveBoost::AdaptiveBoost(const Samples<float>& samples, const float threshold) {
	const float* categories = samples.getCategories();

	int featureNum = samples.getFeatureNum();
	int sampleNum = samples.getSampleNum();
	std::pair<float, float>* featureRanges = new std::pair<float, float>[featureNum];
	for(int j = 0; j < featureNum; ++j) {
		float featureJMin = std::numeric_limits<float>::max();
		float featureJMax = std::numeric_limits<float>::min();
		for(int i = 0; i < sampleNum; ++i) {
			float feature = samples.getSample(i)[j];
			if(featureJMin > feature)
				featureJMin = feature;
			if(featureJMax < feature)
				featureJMax = feature;
		}

		featureRanges[j].first = featureJMin;
		featureRanges[j].second = featureJMax;
	}
	
	int stepNum = 4;
	float* weights = new float[sampleNum];
	for(int i = 0; i < sampleNum; ++i)
		weights[i] = 1.0/sampleNum;

	float errorRate = 0.0;
	do {
#ifdef AB_DEBUG
		for(int i = 0; i < sampleNum; ++i)
			std::cout<<weights[i]<<" ";
		std::cout<<std::endl;
#endif
		WeakLearner weakLearner;
		errorRate = weakLearner.train(samples, categories, featureRanges,
			stepNum, weights);
#ifdef AB_DEBUG
		std::cout<<"<errorRate = "<<errorRate<<">"<<std::endl;
#endif
		weakLearners.push_back(weakLearner);
	} while(errorRate >= threshold);

	delete[] featureRanges;
	delete[] weights;
}

float AdaptiveBoost::classify(const float* sample)const {
	float voted = 0.0;

	int weakLearnerNum = weakLearners.size();
	for(int i = 0; i < weakLearnerNum; ++i) {
		WeakLearner weakLearner(weakLearners[i]);
#ifdef AB_DEBUG
		int feature = weakLearner.getFeature();
		double threshold = weakLearner.getThreshold();
		float predicted = WeakClassifier::classify(sample, feature, threshold);
		std::cout<<"feature = "<<feature<<", threshold = "<<threshold<<", predicted = "
			<<predicted<<std::endl;

		float alpha = weakLearner.getAlpha();
		std::cout<<"alpha = "<<alpha<<std::endl;
		voted += alpha * predicted;
#else
		voted += weakLearner.getAlpha() * WeakClassifier::classify(sample,
			weakLearner.getFeature(), weakLearner.getThreshold());
#endif
	}
#ifdef AB_DEBUG
	std::cout<<"voted = "<<voted<<std::endl;
#endif

	return voted > 0.0 ? 1.0 : -1.0;
}

AdaptiveBoost::~AdaptiveBoost() {
}
