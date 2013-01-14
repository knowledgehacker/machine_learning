#include "FileTokenizer.h"
#include <cstring>
#include <stdexcept>

FileTokenizer::FileTokenizer(const std::string& fileName): dataStream(fileName.c_str()) {
	if(!dataStream)
		throw std::runtime_error("Unable to open file " + fileName + "!");

	dataStream.getline(line, 256);
	offset = 0;
	length = strlen(line);
}

bool FileTokenizer::hasNext() {
	if(offset >= length) {
		dataStream.getline(line, 256);
		if(dataStream.eof() == true)
			return false;

		offset = 0;
		length = strlen(line);
	}

	int i = 0;
	while(offset < length) {
		if(line[offset] == ' ')
			break;

		token[i++] = line[offset++];
	}
	token[i] = '\0';
	// skip following spaces
	while((offset < length) && (line[++offset] == ' '));

	return true;
}

std::string FileTokenizer::nextToken()const {
	return std::string(token);
}


FileTokenizer::~FileTokenizer() {
	dataStream.close();
}	

