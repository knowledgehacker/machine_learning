#include "Samples.h"
#include <stdexcept>
#include <fstream>
#include <iostream>

template<typename DataType>
Samples<DataType>::Samples(const std::string& file, const DataReader<DataType>& reader): dataFile(file), dataReader(reader), 
	featureNum(0), sampleNum(0) {
	std::ifstream dataStream(dataFile.c_str());
	if(!dataStream)
		throw std::runtime_error("Unable to open file " + dataFile + "!");

	char line[256];
	dataStream.getline(line, 256);
	std::string str(line);
	unsigned int start = 0;
	for(unsigned int end = str.find_first_of('\t', start); end != std::string::npos; start = end+1, end = str.find_first_of('\t', start)) {
		std::string feature = str.substr(start, end-start);
		featureNames.push_back(feature);
		++featureNum;
	}
	featureNames.push_back(str.substr(start, str.length()-start));
#ifdef SAMPLES_DEBUG
	std::cout<<"featureNum = "<<featureNum<<std::endl;
	for(int i = 0; i < featureNum; ++i)
		std::cout<<featureNames[i]<<"\t";
	std::cout<<std::endl;
	std::cout<<"category = "<<featureNames[featureNum]<<std::endl;
#endif

	while(1) {
		dataStream.getline(line, 256);
		if(dataStream.bad()) {
			dataStream.close();
			throw std::runtime_error("Unrecoverable errors detected when reading from file " + dataFile + "!");
		}

		if(dataStream.fail() && !dataStream.eof()) {
			dataStream.close();
			throw std::runtime_error("File " + dataFile + " is corrupted!");
		}

		if(dataStream.eof()){
			dataStream.close();
			break;
		}

		DataType* sample = new DataType[featureNum+1];
		if(readSample(line, sample) == false)
			throw std::runtime_error("File " + dataFile + " is corrupted!");
		samples.push_back(sample);
		++sampleNum;
	}
	
	categories = new DataType[sampleNum];
	for(int i = 0; i < sampleNum; ++i)
		categories[i] = samples[i][featureNum];
}

template<typename DataType>
int Samples<DataType>::getFeatureNum()const {
	return featureNum;
}

template<typename DataType>
std::string Samples<DataType>::getFeatureName(const int index)const throw(IndexOutOfBound) {
	if(index >= featureNum)
		throw IndexOutOfBound(index);

	return featureNames[index];
}

template<typename DataType>
const DataType* Samples<DataType>::getCategories()const {
	return categories;
}

template<typename DataType>
int Samples<DataType>::getSampleNum()const {
	return sampleNum;
}
/*
template<typename DataType>
const DataType* Samples<DataType>::getSample(const int index)const throw(IndexOutOfBound) {
	if(index >= sampleNum)
		throw IndexOutOfBound(index);

	return samples[index];
}
*/
template<typename DataType>
bool Samples<DataType>::readSample(char line[256], DataType* sample) {
	std::string str(line);
#ifdef SAMPLES_DEBUG
	std::cout<<"str = "<<str<<std::endl;
#endif

	DataType digit;
	int digitNum = 0;
	size_t start = 0;
	for(size_t end = str.find_first_of('\t', start); (end != std::string::npos) && (digitNum <= featureNum); 
		start = end+1, end = str.find_first_of('\t', start)) {
		std::string digitStr = str.substr(start, end-start);
		if(dataReader(digitStr, digit) == false)
			return false;
		sample[digitNum++] = digit;
	}
	if(dataReader(str.substr(start, str.length()-start), digit) == false)
		return false;
	sample[digitNum] = digit;

#ifdef SAMPLES_DEBUG
	std::cout<<"digitNum = "<<digitNum<<std::endl;
#endif

	return true;
}

template<typename DataType>
Samples<DataType>::~Samples() {
	if(sampleNum > 0) {
		for(typename std::vector<DataType*>::iterator iter = samples.begin(); iter != samples.end(); ++iter)
			delete[] *iter;

		delete[] categories;
	}
}
