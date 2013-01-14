#include "Sigmoid.h"
#include "Utils.h"
#ifdef LR_DEBUG
#include <iostream>
#endif

/*
 * sigmoid(z) = 1 /(e^(-z) + 1).
 */
long double Sigmoid::operator()(const float* sample, const long double* params, int len)const {
	long double z = 0.0;
	for(int i = 0; i < len; ++i)
		z += sample[i] * params[i];
#ifdef LR_DEBUG
	//std::cout<<"z = "<<z<<std::endl;
#endif

	return 1 / (pow(-z) + 1);
}
