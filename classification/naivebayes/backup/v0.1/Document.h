#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include <string>

class Document {
private:
	std::string name;
	int category;

public:
	Document(const std::string& docName, const int docCategory);
	std::string getName()const {
		return name;
	}
	int getCategory()const {
		return category;
	}
	~Document();
};

#endif
