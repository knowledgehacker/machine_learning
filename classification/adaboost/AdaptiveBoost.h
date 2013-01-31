/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _ADAPTIVE_BOOST_H_
#define _ADAPTIVE_BOOST_H_

#include "WeakLearner.h"
#include <vector>

class AdaptiveBoost {
private:
	std::vector<WeakLearner> weakLearners;

public:
	AdaptiveBoost(const Samples<float>& samples, const float threshold);
	float classify(const float* sample)const;
	~AdaptiveBoost();
};

#endif
