/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _SAMPLES_H_
#define _SAMPLES_H_

#include "IndexOutOfBound.h"
#include "DataReader.h"
#include <string>
#include <vector>

/*
 * Class Samples reads trainging or test samples from file and store them in a vector.
 * To read samples stored in file, user-defined converter should be provided to translate the string into data as expected.
 * Users can define their converter by inherting from interface Converter.
 */
template<typename DataType>
class Samples {
private:
	std::string dataFile;
	const DataReader<DataType>& dataReader;
	std::vector<std::string> featureNames;
	int featureNum;	// number of features, excluding class label
	std::vector<DataType*> samples;
	int sampleNum;
	DataType* categories;

public:
	Samples(const std::string& file, const DataReader<DataType>& reader);
	int getFeatureNum()const;
	std::string getFeatureName(const int index)const throw(IndexOutOfBound);
	const DataType* getCategories()const;
	int getSampleNum()const;
	inline const DataType* getSample(const int index)const throw(IndexOutOfBound) {
	    if(index >= sampleNum)
    	    throw IndexOutOfBound(index);

	    return samples[index];
	}
	virtual ~Samples();

protected:
	bool readSample(char line[256], DataType* sample);
};

#endif
