#include "InputDataSet.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdexcept>
#include <iostream>

InputDataSet::InputDataSet(const std::string& dir): dirName(dir), categoryNum(0) {
	readDir();
}

std::string& InputDataSet::getDir() {
	return dirName;
}

std::vector<std::string>& InputDataSet::getDocuments(const int category) throw(IndexOutOfBound) {
	if(category >= categoryNum)
		throw IndexOutOfBound(category);

	return docs[category];
}

int InputDataSet::getCategoryNum()const {
	return categoryNum;
}

void InputDataSet::readDir() {
	DIR* dir = opendir(dirName.c_str());
	struct dirent* dirEntry = readdir(dir);
	while(dirEntry != 0) {
		std::string fileName(dirEntry->d_name);
#ifdef NBC_DEBUG
		std::cout<<"fileName = "<<fileName<<std::endl;
#endif
		if((fileName != ".") && (fileName != "..") && (fileName != ".directory")) {
			int category = parse(fileName);
			docs[category].push_back(fileName);
			if(categoryNum < (category+1))
				categoryNum = category+1;
		}

		dirEntry = readdir(dir);
	}
	closedir(dir);
}

int InputDataSet::parse(const std::string& fileName)const {
	size_t pos = fileName.find_first_of('_', 0);
	std::string categoryName = fileName.substr(0, pos);
	if(categoryName[0] != 'C')
		throw std::runtime_error("Invalid document name " + fileName + "!");
	int category = 0;
	for(size_t i = 1; i < pos; ++i) {
		char c = categoryName[i];
		if(!((c >= '0') && (c <= '9')))
			throw std::runtime_error("Invalid document name " + fileName + "!");
		category += category*10 + c - '0';
	}
#ifdef NBC_DEBUG
	std::cout<<"categoryName = "<<categoryName<<", category = "<<category<<std::endl;
#endif
	return category;
}

InputDataSet::~InputDataSet() {
}
