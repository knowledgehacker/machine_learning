#include "InputDataSet.h"
#include <stdexcept>
#include <fstream>
#include <iostream>

InputDataSet::InputDataSet(const std::string& fileName): dataFile(fileName), itemNum(0) {
	std::ifstream dataStream(dataFile.c_str());
	if(!dataStream)
		throw std::runtime_error("Unable to open file " + dataFile + "!");

	char line[256];
	dataStream.getline(line, 256);
	
	int fn = 0;
	std::string str(line);
	unsigned int start = 0;
	for(unsigned int end = str.find_first_of('\t', start); end != std::string::npos; start = end+1, end = str.find_first_of('\t', start)) {
		std::string feature = str.substr(start+1, (end-1)-(start+1));	// strip away ""
		features.push_back(feature);
		++fn;
	}
	features.push_back(str.substr(start+1, (str.length()-1)-(start+1)));
	Item::featureNum = fn+1;
#ifdef IDS_DEBUG
	for(unsigned int i = 0; i < features.size(); ++i)
		std::cout<<features[i]<<"\t";
	std::cout<<std::endl;
	std::cout<<"Item::featureNum = "<<Item::featureNum<<std::endl;
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

		Item item(itemNum);
		if(false == readItem(line, item))
			throw std::runtime_error("File " + dataFile + " is corrupted!");
		items.push_back(item);
		++itemNum;
	}	
}

std::string InputDataSet::getFeature(const int index)const throw(IndexOutOfBound) {
	if(index >= (Item::featureNum-1))
		throw IndexOutOfBound(index);

	return features[index];
}

int InputDataSet::getItemNum()const {
	return itemNum;
}

const Item& InputDataSet::operator[](const int index)const throw(IndexOutOfBound) {
	if(index >= itemNum)
		throw IndexOutOfBound(index);

	return items[index];
}

bool InputDataSet::readItem(char line[256], Item& item) {
	std::string str(line);
#ifdef IDS_DEBUG
	std::cout<<"str = "<<str<<std::endl;
#endif

	int i = 0;
	size_t start = str.find_first_of('"');
	if(start == std::string::npos)
		return false;
	size_t end = str.find_first_of('"', start+1);
	if(end == std::string::npos)
		return false;

	while(1) {
#ifdef IDS_DEBUG
		std::cout<<"str.substr("<<start+1<<", "<<end-(start+1)<<") = "<<str.substr(start+1, end-(start+1))<<std::endl;
#endif
		item[i++] = str.substr(start+1, end-(start+1));
		start = str.find_first_of('"', end+1);
		if(start == std::string::npos)
			break;
		end = str.find_first_of('"', start+1);
		if(end == std::string::npos)
			return false;
	}
#ifdef IDS_DEBUG
	std::cout<<"i = "<<i<<std::endl;
#endif
	if(i != Item::featureNum)
		return false;

	return true;
}

InputDataSet::~InputDataSet() {
}
