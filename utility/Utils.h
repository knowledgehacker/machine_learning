#ifndef _UTILS_H_

float ln(float n);

/*
 * The natural logarithm in C++, that is, function 'log' defined in cmatch, is the base-e logarithm.
 * Since logN(x) = logY(x)/logY(N), we can calculate any base logarithm using the base-e logrithm.
 * For example, log2(x) = log(x)/log(2).
 */
float logN(float n, int base);

/*
 * e^z = (2^logN(e, 2))^z = 2^(z / logN(e, 2));
 */
float pow(int z);
float pow(float z);
long double pow(long double z);

float min(float x, float y);
float max(float x, float y);

#endif
