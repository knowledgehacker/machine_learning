/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _FLOAT_DATA_READER_H_
#define _FLOAT_DATA_READER_H_

#include "DataReader.h"

class FloatDataReader: public DataReader<float> {
public:
	bool operator()(const std::string& digitStr, float& value)const;
};

#endif
