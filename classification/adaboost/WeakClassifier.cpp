#include "WeakClassifier.h"

float WeakClassifier::classify(const float* sample, const int feature, const float threshold) {
    return sample[feature] > threshold ? 1.0 : -1.0;    // categories: -1, 1
}
