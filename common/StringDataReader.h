/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _STRING_DATA_READER_H_
#define _STRING_DATA_READER_H_

#include "DataReader.h"

class StringDataReader: public DataReader<std::string> {
public:
	bool operator()(const std::string& digitStr, std::string& value)const;
};

#endif
