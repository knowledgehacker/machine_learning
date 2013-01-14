#include "StringDataReader.h"

bool StringDataReader::operator()(const std::string& digitStr, std::string& value)const {
    value = digitStr;

    return true;
}
