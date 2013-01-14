#ifndef _LOGISTIC_REGRESSION_H_
#define _LOGISTIC_REGRESSION_H_

#include "Samples.h"
#include "RegressionFunction.h"

class LogisticRegression {
private:
	Samples<float> trainingSamples;
	int featureNum;
	int sampleNum;
	float* categories;
	float categoryRange;
	const RegressionFunction& rf;
	float threshold;

	float alpha;
	long double* parameters;

public:
	LogisticRegression(const Samples<float>& samples, const float a, const RegressionFunction& regFunc, float threshold);
	int classify(const float* testSample);
	~LogisticRegression();

private:
	// return: iterations to reach convergence.
	int gradientAscent();
	void stochasticGradientAscent();
	bool convergent(const long double* deviations, const float categoryRange)const;
};

#endif
