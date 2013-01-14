#include "OfflineNaiveBayesClassifier.h"
#include "FileTokenizer.h"
#include <iostream>

int main(int argc, char** argv) {
	std::string trainingDir = "data//training";
	std::string doc = "data//test//posting.txt";
/*
	FileTokenizer tokenizer(doc);
	while(tokenizer.hasNext()) {
		std::string token = tokenizer.nextToken();
		std::cout<<token<<std::endl;
	}
*/
	InputDataSet inputDataSet(trainingDir);
	OfflineNaiveBayesClassifier offlineNBC(inputDataSet);
	offlineNBC.classify(doc);

	return 0;
}
