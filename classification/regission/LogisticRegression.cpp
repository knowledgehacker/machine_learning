/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "LogisticRegression.h"
#include "Samples.cpp"
#include <cmath>
#ifdef LR_DEBUG
#include <iostream>
#endif

LogisticRegression::LogisticRegression(const Samples<float>& samples, const float a,
	const RegressionFunction& regFunc, float thres): trainingSamples(samples), 
	featureNum(samples.getFeatureNum()), sampleNum(samples.getSampleNum()), 
	alpha(a), rf(regFunc), threshold(thres) {
	if((featureNum <= 0) || (sampleNum <= 0)) {
		// TODO: handle scenario "featureNum <= 0" and "sampleNum <= 0" in proper way.
	}

	categories = new float[sampleNum];
	for(int i = 0; i < sampleNum; ++i)
		categories[i] = trainingSamples.getSample(i)[featureNum];

	// calculate category range = maximum category - minimum category
	float category0 = categories[0];
	float minCategory = category0;
	float maxCategory = category0;
	for(int i = 1; i < sampleNum; ++i) {
		float categoryi = categories[i];
		if(minCategory > categoryi)
			minCategory = categoryi;

		if(maxCategory < categoryi)
			maxCategory = categoryi;
	}
	categoryRange = maxCategory - minCategory;
#ifdef LR_DEBUG
	std::cout<<"categoryRange = "<<categoryRange<<std::endl;
#endif

	parameters = new long double[featureNum];
	for(int i = 0; i < featureNum; ++i)
		parameters[i] = 1.0;

	// calculate parameters using gradient ascent algorithm
	gradientAscent();
}

int LogisticRegression::classify(const float* sample) {
	long double rfValue = rf(sample, parameters, featureNum);
	int category = 0;
	long double minDeviation = fabs(rfValue - categories[0]);
	for(int i = 1; i < sampleNum; ++i) {
		long double deviation = fabs(rfValue - categories[i]);
		if(deviation > minDeviation) {
			category = i;
			minDeviation = deviation;
		}
	}

	return category;
}

int LogisticRegression::gradientAscent() {
	long double* deviations = new long double[sampleNum];
	int iterations = 0;
	do {
		++iterations;

		/*
		 * How to handle the case that 'z' is either too big or too small???
		 */
		// calculate deviations for all samples, one per sample.
		for(int i = 0; i < sampleNum; ++i)
			deviations[i] = categories[i] - rf(trainingSamples.getSample(i), 
				parameters, featureNum);

		// adjust parameters
		for(int j = 0; j < featureNum; ++j) {
			long double factor = 0.0;
			for(int i = 0; i < sampleNum; ++i) 
				factor +=  trainingSamples.getSample(i)[j] * deviations[i];
			parameters[j] += alpha * factor;
		}
	} while(convergent(deviations, categoryRange) == false);
	delete[] deviations;

	return iterations;
}

/*
 * Stochastic gradient ascent algorithm is an improvement on gradient ascent algorithm.
 * The improvements include:
 * 1) For each update of parameters, only one sample instead of all samples is involved,
 * which reduces computation greatly.
 * 2) It's online, when new samples come, parameters can be updated to reflect their impact.
 */
void LogisticRegression::stochasticGradientAscent() {
	for(int i = 0; i < sampleNum; ++i) {
		const float* sample = trainingSamples.getSample(i);
		long double deviation = categories[i] - rf(sample, parameters, featureNum);
		for(int j = 0; j < featureNum; ++j)
			parameters[j] += alpha * deviation * sample[j];
	}
}

bool LogisticRegression::convergent(const long double* deviations,
	const float categoryRange)const {
	for(int i = 0; i < sampleNum; ++i)
		if((fabs(deviations[i]) / categoryRange) >= threshold) {
#ifdef LR_DEBUG
		std::cout<<"i = "<<i<<", fabs(deviations[i]) / categoryRange = "
			<<fabs(deviations[i]) / categoryRange<<std::endl;
#endif
			return false;
		}

	return true;
}

LogisticRegression::~LogisticRegression() {
	if((featureNum > 0) && (sampleNum > 0)) {
		delete[] categories;
		delete[] parameters;
	}
}
