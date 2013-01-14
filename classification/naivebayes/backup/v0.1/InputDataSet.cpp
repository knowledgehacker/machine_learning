#include "InputDataSet.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdexcept>
#include <iostream>

InputDataSet::InputDataSet(const std::string& dir): dirName(dir) {
}

std::string InputDataSet::getDir()const {
	return dirName;
}

std::vector<Document>& InputDataSet::readDir() {
	DIR* dir = opendir(dirName.c_str());
	struct dirent* dirEntry = readdir(dir);
	while(dirEntry != 0) {
		std::string fileName(dirEntry->d_name);
		std::cout<<"fileName = "<<fileName<<std::endl;
		if((fileName != ".") && (fileName != "..") && (fileName != ".directory"))
			docs.push_back(parse(fileName));

		dirEntry = readdir(dir);
	}
	closedir(dir);
	
	return docs;
}

Document InputDataSet::parse(const std::string& fileName)const {
	size_t pos = fileName.find_first_of('_', 0);
	std::string category = fileName.substr(0, pos);
	int index = 0;
	if(category[0] != 'C')
		throw std::runtime_error("Invalid document name " + fileName + "!");
	for(size_t i = 1; i < pos; ++i) {
		if(!((category[i] >= '0') && (category[i] <= '9')))
			throw std::runtime_error("Invalid document name " + fileName + "!");
		index += index*10 + category[i] - '0';
	}
	//std::cout<<"category = "<<category<<", index = "<<index<<std::endl;

	return Document(fileName, index);
}

InputDataSet::~InputDataSet() {
}
