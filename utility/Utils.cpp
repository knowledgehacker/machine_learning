#include "Utils.h"
#include <cmath>

float ln(float n) {
	return log(n);
}

float logN(float n, int base) {
	return log(n)/log(base);
}

float pow(int z) {
	return pow(2, z / log(2));
}

float pow(float z) {
	return pow(2, z / log(2));
}

long double pow(long double z) {
	return pow(2, z / log(2));
}

float min(float x, float y) {
	return x < y ? x : y;
}

float max(float x, float y) {
	return min(y, x);
}
