#include "InputDataSet.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

InputDataSet::InputDataSet(std::string dir) : dataDir(dir) {
	for(int i = 0; i < DIGIT_NUM; ++i) 
		for(int j = 0; ; ++j) {
			std::ostringstream sampleFile;
			sampleFile<<dataDir<<"/"<<i<<"_"<<j<<".txt";
			//std::cout<<sampleFile.str()<<std::endl;
	
#ifdef CHECK_IO
			std::bitset<1024> sample;
			if(!readSample(sampleFile.str(), sample))
				break;
			digits[i].push_back(sample);
#else
			std::ostringstream sample;
			if(!readSample(sampleFile.str(), sample))
				break;
			digits[i].push_back(std::bitset<1024>(sample.str()));
#endif
		}

	nextIdx = 0;
}

bool InputDataSet::hasNext() {
	if(nextIdx == DIGIT_NUM) {
		nextIdx = 0;
		return false;
	}

	return true;
}

std::vector<std::bitset<1024> >& InputDataSet::next() {
	return digits[nextIdx++];
}

#ifdef CHECK_IO
bool InputDataSet::readSample(std::string sampleFile, std::bitset<1024>& sample) {
#else
bool InputDataSet::readSample(std::string sampleFile, std::ostringstream& sample) {
#endif
	std::ifstream sampleStream(sampleFile.c_str());
	if(!sampleStream) 
    	return false;

#ifdef CHECK_IO
	char line[256];
	int lineNo = 0;
	while(1) {
		sampleStream.getline(line, 256);
		if(sampleStream.bad()){
			sampleStream.close();
			throw std::runtime_error("Unercoverable error detected when reading from file " + sampleFile + "!");
		}
		if(sampleStream.fail() && !sampleStream.eof()){
			sampleStream.close();
			throw std::runtime_error("File " + sampleFile + " corrupted!");
		}

		if(sampleStream.eof()){
			sampleStream.close();
			break;
		}

		for(int i = 0; i < SAMPLE_DIM; ++i){
			char c = line[i];
			if(c != '0') 
				sample[lineNo*SAMPLE_DIM+i] = c - '0';
		}

		++lineNo;
	}
#else
	std::string line;
	while(!(sampleStream>>line).eof())
		sample<<line;
	sampleStream.close();
#endif

	return true;
}

InputDataSet::~InputDataSet() {
}
