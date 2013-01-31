/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "FloatDataReader.h"

bool FloatDataReader::operator()(const std::string& digitStr, float& value)const {
	float digit = 0.0;

	bool minus = false;
    size_t dotPos = digitStr.find_first_of('.', 0);
    if(dotPos == std::string::npos) {
		size_t i = 0;
		if(digitStr[i] == '-') {
			minus = true;
			++i;
		} else if(digitStr[i] == '+')
			++i;
		
        while(i < digitStr.length())
            digit += digit*10 + digitStr[i++] - '0';
    } else {
        if(dotPos == digitStr.length())
            return false;

        std::string integralStr = digitStr.substr(0, dotPos);
        std::string decimalStr = digitStr.substr(dotPos+1, digitStr.length()-(dotPos+1));

        int integral = 0.0;
		size_t i = 0;
		if(integralStr[i] == '-') {
			minus = true;
			++i;
		} else if(integralStr[i] == '+')
			++i;
	
        while(i < integralStr.length())
            integral = integral*10 + integralStr[i++] - '0';

        float decimal = 0.0;
        int denominator = 10;
        for(size_t j = 0; j < decimalStr.length(); ++j) {
            decimal = decimal + static_cast<float>(decimalStr[j] - '0') / denominator;
            denominator *= 10;
        }

        digit = integral + decimal;
    }
    value = (minus == true) ? -digit : digit;

    return true;
}
