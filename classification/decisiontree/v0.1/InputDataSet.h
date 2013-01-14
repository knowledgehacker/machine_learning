#ifndef _INPUT_DATA_SET_H_
#define _INPUT_DATA_SET_H_

#include "Item.h"
#include <string>
#include <vector>

/*
 * Read data from file and parse it into Items.
 * The file conforms to the following format:
 * 1) The first line list the names of features and a label separated by TAB '\t'. The label follows all the features.
 * 2) The following lines list all the items, one item per line. Each item consists of values of features and a label, separated by TAB '\t'.
 */
class InputDataSet {
private:
	std::string dataFile;
	std::vector<std::string> features;
	std::vector<Item> items;
	int itemNum;

public:
	// read data from all files in directory 'dataDir'
	//InputDataSet(const std::string& dataDir);
	// read data from file 'dataFile'
	InputDataSet(const std::string& fileName);
	std::string getFeature(const int index)const throw(IndexOutOfBound);
	int getItemNum()const;	
	const Item& operator[](const int index)const throw(IndexOutOfBound);
	~InputDataSet();

private:
	bool readItem(char line[256], Item& item);
};

#endif
