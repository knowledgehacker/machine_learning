#include "Document.h"

Document::Document(const std::string& docName, const int docCategory): name(docName), category(docCategory) {
}

/*
inline std::string Document::getName()const {
	return name;
}

inline int Document::getCategory()const {
	return category;
}
*/

Document::~Document() {
}
