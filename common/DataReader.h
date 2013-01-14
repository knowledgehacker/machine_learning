#ifndef _DATA_READER_H_
#define _DATA_READER_H_

#include <string>

template<typename DataType>
class DataReader {
public:
	virtual bool operator()(const std::string& digitStr, DataType& value)const = 0;
};

#endif
