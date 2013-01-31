/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _FILE_TOKENIZER_H_
#define _FILE_TOKENIZER_H_

#include <string>
#include <fstream>

class FileTokenizer {
private:
	std::ifstream dataStream;
	char line[256];
	int offset;
	int length;

	// next token 	
	char token[256];

public:
	FileTokenizer(const std::string& fileName);
	bool hasNext();
	std::string nextToken()const;
	~FileTokenizer();
};

#endif
