#ifndef _INPUT_DATA_SET_H_
#define _INPUT_DATA_SET_H_

#include "Document.h"
#include <string>
#include <vector>

class InputDataSet {
private:
	std::string dirName;
	std::vector<Document> docs;

public:
	InputDataSet(const std::string& dir);
	std::string getDir()const;
	std::vector<Document>& readDir();
	~InputDataSet();

private:
	Document parse(const std::string& fileName)const;
};

#endif
