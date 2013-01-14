#ifndef _SIGMOID_H_
#define _SIGMOID_H_

#include "RegressionFunction.h"

class Sigmoid: public RegressionFunction {
public:
	long double operator()(const float* sample, const long double* params, int len)const;
};

#endif
