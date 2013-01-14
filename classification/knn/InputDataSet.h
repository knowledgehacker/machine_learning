#ifndef _INPUT_DATA_SET_H_
#define _INPUT_DATA_SET_H_

#include <string>
#include <bitset>
#include <vector>
#ifndef CHECK_IO
#include <sstream>
#endif

class InputDataSet {
public:
	static const int DIGIT_NUM = 10;
	static const int SAMPLE_DIM = 32;

private:
	std::string dataDir;
	std::vector<std::bitset<1024> > digits[DIGIT_NUM];
	int nextIdx;

public:
	InputDataSet(std::string dir);
	bool hasNext();
	std::vector<std::bitset<1024> >& next();
	~InputDataSet();

private:
#ifdef CHECK_IO
	bool readSample(std::string sampleFile, std::bitset<1024>& sample);
#else
	bool readSample(std::string sampleFile, std::ostringstream& sample);
#endif
};

#endif
