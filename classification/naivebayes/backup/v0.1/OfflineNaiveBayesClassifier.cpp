#include "OfflineNaiveBayesClassifier.h"
#include "FileTokenizer.h"
#include "Utils.h"
#include "Vector.cpp"
#include <iostream>

OfflineNaiveBayesClassifier::OfflineNaiveBayesClassifier(InputDataSet& inputDataSet): documents(inputDataSet.readDir()) {
	documentNum = documents.size();
	if(documentNum <= 0)
		return;

	std::string dirName = inputDataSet.getDir() + "//";
	// construct vocabulary
	categoryNum = 0;
	vocabularyNum = 0;
	for(int i = 0; i < documentNum; ++i) {
		Document doc = documents[i];
		if(categoryNum < doc.getCategory())
			categoryNum = doc.getCategory();

		FileTokenizer tokenizer(dirName + doc.getName());
		while(tokenizer.hasNext()) {
			std::string token = tokenizer.nextToken();
			std::map<std::string, int>::iterator pos = vocabulary.find(token);
			if(pos == vocabulary.end())
				vocabulary[token] = vocabularyNum++;
		}
	}
	++categoryNum;
	std::cout<<"categoryNum = "<<categoryNum<<", vocabularyNum = "<<vocabularyNum<<std::endl;
	
	// calcualte the probabilities of document categories, one probability per category
	categoryProbabilities = new double[categoryNum];
	Document doc = documents[0];
	std::string docName = doc.getName();
	int category = doc.getCategory();
	int cnt = 1;
	for(int i = 1; i < documentNum; ++i) {
		if(documents[i].getCategory() != category) {
			categoryProbabilities[category] = cnt/documentNum;
			std::cout<<"categoryProbabilities["<<category<<"] = "<<categoryProbabilities[category]<<std::endl;
	
			cnt = 1;
			category = documents[i].getCategory();
		}else {
			++cnt;
		}
	}

	// calculate the vectors for all the documents, one vector per document
	for(int i = 0; i < documentNum; ++i) {
		int* docVector = new int[vocabularyNum];
		//Vector<int> docVector(vocabularyNum);
		for(int j = 0; j < vocabularyNum; ++j)
			docVector[j] = 0;

		std::string docName = documents[i].getName();
		FileTokenizer tokenizer(dirName + docName);
		int docTokens = 0;
		while(tokenizer.hasNext()) {
			/*
			std::map<std::string, int>::iterator pos = vocabulary.find(tokenizer.nextToken());
			if(pos != vocabulary.end())
				docVector[(*pos).second] = 1;
			*/
			// This is an offline version, so vocabulary contains all the words in document 'docToClassify'.
			docVector[vocabulary[tokenizer.nextToken()]] = 1;

			++docTokens;
		}
		documentVectors[docName] = docVector;
		documentTokens[docName] = docTokens;
	}
}

int OfflineNaiveBayesClassifier::classify(const std::string& docToClassify) {
	double* docProbabilities = new double[categoryNum];
	for(int j = 0; j < categoryNum; ++j)
		docProbabilities[j] = 1.0;

	int totalDocTokens = 0;
	for(std::map<std::string, int>::iterator iter = documentTokens.begin(); iter != documentTokens.end(); ++iter)
		totalDocTokens += (*iter).second;

	int* freq = new int[categoryNum];
	// docTokens[i] is the number of tokens in the documents belong to category 'i'.
	int* docTokens = new int[categoryNum];
	FileTokenizer tokenizer(docToClassify);
	while(tokenizer.hasNext()) {
		for(int j = 0; j < categoryNum; ++j)
			freq[j] = 0;
		for(int j = 0; j < categoryNum; ++j)
			docTokens[j] = 0;

		std::string token = tokenizer.nextToken();
		for(int i = 0; i < documentNum; ++i) {
			Document doc = documents[i];
			std::string docName = doc.getName();
			int category = doc.getCategory();
			if(documentVectors[docName][vocabulary[token]] == 1) {
				++freq[category];
				docTokens[category] += documentTokens[docName];
			}
		}

		for(int i = 0; i < categoryNum; ++i) {
			double prob;
			if(freq[i] == 0)
				prob = logN(categoryProbabilities[i], 2);
			else
				prob = logN(freq[i] / docTokens[i], 2) + logN(categoryProbabilities[i], 2) - logN(freq[i] / totalDocTokens, 2);
			docProbabilities[i] *= prob;
		}
	}
	delete[] freq;
	delete[] docTokens;

	int index = 0;
	double maxProbability = docProbabilities[0];
	for(int i = 1; i < categoryNum; ++i) {
		if(maxProbability < docProbabilities[i]) {
			index = i;
			maxProbability = docProbabilities[i];
		}
	}
	delete[] docProbabilities;

	return index;
}

OfflineNaiveBayesClassifier::~OfflineNaiveBayesClassifier() {
	if(documentNum > 0) {
		delete[] categoryProbabilities;
		
		for(std::map<std::string, int*>::iterator iter = documentVectors.begin(); iter != documentVectors.end(); ++iter) {
			int* value = (*iter).second;
			delete[] value;
		}	
	}
}
