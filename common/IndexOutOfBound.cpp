/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "IndexOutOfBound.h"
#include <sstream>

IndexOutOfBound::IndexOutOfBound(const int index) {
	this->index = index;
}

const char* IndexOutOfBound::what() const throw() {
	std::ostringstream msg;
	msg<<"IndexOutOfBound: "<<index;

	return msg.str().c_str();
}

IndexOutOfBound::~IndexOutOfBound()throw() {
}
