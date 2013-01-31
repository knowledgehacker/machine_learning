/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _INDEX_OUT_OF_BOUND_H_
#define _INDEX_OUT_OF_BOUND_H_

#include <stdexcept>

class IndexOutOfBound: public std::exception {
private:
	int index;

public:
	IndexOutOfBound(const int index);
	const char* what()const throw();
	~IndexOutOfBound()throw();
};

#endif
