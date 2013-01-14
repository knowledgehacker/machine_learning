#ifndef _INPUT_DATA_SET_H_
#define _INPUT_DATA_SET_H_

#include "IndexOutOfBound.h"
#include <string>
#include <vector>
#include <map>

class InputDataSet {
private:
	std::string dirName;
	std::map<int, std::vector<std::string> > docs;
	int categoryNum;

public:
	InputDataSet(const std::string& dir);
	// A member function returns reference to a member variable can't be constant
	std::string& getDir();
	std::vector<std::string>& getDocuments(const int category) throw(IndexOutOfBound);
	int getCategoryNum()const;
	~InputDataSet();

private:
	void readDir();
	int parse(const std::string& fileName)const;
};

#endif
