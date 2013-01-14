#ifndef _WEAK_LEARNER_H_

#include "Samples.h"
#include <utility>

class WeakLearner {
private:
	static float INFINITESIMAL;

	int feature;
	float threshold;
	float alpha;
	
public:
	WeakLearner();
	float train(const Samples<float>& samples, const float* categories, const std::pair<float, float>* featureRanges, 
		const int stepNum, float* weights);
	inline int getFeature()const {
		return feature;
	}
	inline float getThreshold()const {
		return threshold;
	}
	float getAlpha()const {
		return alpha;
	}
	~WeakLearner();
};

#endif
