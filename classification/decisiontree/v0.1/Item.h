#ifndef _ITEM_H_
#define _ITEM_H_

#include "IndexOutOfBound.h"
#include <string>
#include <vector>
#include <ostream>

class Item {
public:	
	// record the number of features and the label
	static int featureNum;

private:
	// no of the item in all items read from data file
	int no;
	// the last feature is label
	std::string* features;

public:
	explicit Item();
	//Item(const std::vector<std::string>& featureList);
	Item(const int idx);
	Item(const Item& item);
	Item& operator=(const Item& item);
	const std::string& getLabel()const;
	std::string& operator[](const int index)const throw(IndexOutOfBound);
	friend std::ostream& operator<<(std::ostream& os, const Item& item);
	~Item();
};

#endif
