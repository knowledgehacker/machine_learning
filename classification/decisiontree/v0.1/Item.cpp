#include "Item.h"

// it is set when the first line of the dataFile is read
int Item::featureNum = 0;

Item::Item(): no(-1), features(0) {
}

Item::Item(const int idx): no(idx) {
	try{
		features = new std::string[featureNum];
	} catch(std::bad_alloc except) {
		// ...
	}
}

/*
 * Implementation of copy constructor here is incorrect.
 * We should free the old memory occupied by features and allocate a new one for it before coping data from 'item'. Otherwise the following code
 * snippet will cause memory leak:
 *
 * Item item(6);
 * Item aitem(5);	
 * aitem = item;	// memory leak
 *
 * But Item serves as the type of elements stored in std::vector, std::vector initializes a temporary Item object using default Item constructor, 
 * and uses Item's copy constructor to copy the temporary Item object to every element when initializing elements in the vector. 
 * So "free the old memory and allocate a new one" doesn't work in such scenario, since there is no old memory to free at all at that time.
 * But if you call std::vector's default constructor to create a std::vector object, and add elements using std::vector.push_back, then no 
 * initialization happends, std::vector.push_back using the paramter's copy constructor to copy the element directly.
 *
 * Does it mean we can not store objects with dynamically allocated array resources in a STL container?
 * Here as a temporary solution, we do not free the old memory here, it works here since we do not use Item objects like the code mentioned above.
 */
Item::Item(const Item& item) {
	no = item.no;

	if(item.features == 0)
		features = 0;
	else {
		try{
			features = new std::string[featureNum];
		} catch(std::bad_alloc except) {
			// ...
		}
		for(int i = 0; i < featureNum; ++i)
			features[i] = item.features[i];
	}
}

Item& Item::operator=(const Item& item) {
	if(this != &item) {
		no = item.no;

		if(item.features == 0)
			features = 0;
		else {
			if(features != 0)
				delete[] features;
			try{
				features = new std::string[featureNum];
			} catch(std::bad_alloc except) {
				// ...
			}
			for(int i = 0; i < featureNum; ++i)
				features[i] = item.features[i];
		}
	}

	return *this;
}

const std::string& Item::getLabel()const {
	return features[featureNum-1];
}

std::string& Item::operator[](const int index)const throw(IndexOutOfBound) {
	if(index >= featureNum)
		throw IndexOutOfBound(index);

	return features[index];
}

std::ostream& operator<<(std::ostream& os, const Item& item) {
	os<<item.no<<". ";
	os<<"(";
	int fn = Item::featureNum;
	int i = 0;
	while(i < (fn-1))
		os<<item[i++]<<", ";
	os<<item[i];
	os<<")";

	return os;
}

Item::~Item() {
	delete[] features;
}
