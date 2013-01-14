#include "OfflineNaiveBayesClassifier.h"
#include "FileTokenizer.h"
#include "Utils.h"
#include "Vector.cpp"
#include <iostream>

OfflineNaiveBayesClassifier::OfflineNaiveBayesClassifier(InputDataSet& inputDataSet): categoryNum(inputDataSet.getCategoryNum()) {
	if(categoryNum <= 0)
		return;
	
	int documentNum = 0;
	documents = new std::vector<std::string>[categoryNum];
	for(int category = 0; category < categoryNum; ++category) {
		documents[category] = inputDataSet.getDocuments(category);
		documentNum += documents[category].size();
	}

	// construct vocabulary 
	std::string dirName = inputDataSet.getDir() + "//";
	vocabularyNum = 0;
	for(int i = 0; i < categoryNum; ++i) {
		std::vector<std::string>& cateDocuments = documents[i];
		for(std::vector<std::string>::const_iterator iter = cateDocuments.begin(); iter != cateDocuments.end(); ++iter) {
			FileTokenizer tokenizer(dirName + *iter);
			while(tokenizer.hasNext()) {
				std::string token = tokenizer.nextToken();
				std::map<std::string, int>::iterator pos = vocabulary.find(token);
				if(pos == vocabulary.end())
					vocabulary[token] = vocabularyNum++;
			}
		}
	}
#ifdef NBC_DEBUG
	std::cout<<"vocabularyNum = "<<vocabularyNum<<std::endl;
	for(std::map<std::string, int>::const_iterator iter = vocabulary.begin(); iter != vocabulary.end(); ++iter)
		std::cout<<"("<<(*iter).first<<", "<<(*iter).second<<") ";
	std::cout<<std::endl;
#endif
	
	/*
	 * Here we calculate vectors for all the docuemts, one vector per document.
	 * Actually if we only need to collect how many times of each token in all the documents, 
	 * we only need to calculate vectors based on category, one vector per category.
	 */
#ifdef USE_VECTOR
	documentVectors = new std::map<std::string, Vector<int> >[categoryNum];
#else 
	documentVectors = new std::map<std::string, int*>[categoryNum];
#endif
	documentTokens = new std::map<std::string, int>[categoryNum];
	for(int i = 0; i < categoryNum; ++i) {
		std::vector<std::string>& cateDocuments = documents[i];
		for(std::vector<std::string>::const_iterator iter = cateDocuments.begin(); iter != cateDocuments.end(); ++iter) {
#ifdef USE_VECTOR
			Vector<int> docVector(vocabularyNum);
#else
			int* docVector = new int[vocabularyNum];
#endif
			for(int j = 0; j < vocabularyNum; ++j)
				docVector[j] = 0;

			std::string docName = *iter;
#ifdef NBC_DEBUG
			std::cout<<"name: "<<docName<<std::endl;
#endif
			FileTokenizer tokenizer(dirName + docName);
			int docTokens = 0;
			while(tokenizer.hasNext()) {
				/*
				 * This is an offline version, vocabulary contains all the words in document 'docToClassify'.
				 * So we don't need to check whether the token exists in the vocabulary or not.
				 */
#ifdef NBC_DEBUG
				std::string token = tokenizer.nextToken();
				std::cout<<"token = "<<token<<std::endl;
				docVector[vocabulary[token]] = 1;
#endif
				docVector[vocabulary[tokenizer.nextToken()]] = 1;

				++docTokens;
			}
			// Question: Why Vector's copy constructor is called twice when the following statement is executed?
#ifdef NBC_DEBUG
			std::cout<<"before documentVectors[i][docName] = docVector;"<<std::endl;
#endif
			documentVectors[i][docName] = docVector;
#ifdef NBC_DEBUG
			std::cout<<"after documentVectors[i][docName] = docVector;"<<std::endl;
#endif

			documentTokens[i][docName] = docTokens;
#ifdef NBC_DEBUG
			for(int j = 0; j < vocabularyNum; ++j)
				std::cout<<documentVectors[i][docName][j]<<" ";
			std::cout<<std::endl;

			std::cout<<"length: "<<documentTokens[i][docName]<<std::endl;;
#endif	
		}
	}	
}

/*
 * Here calculate the probabilities of document 'docToClassify' belonging to class 'Ci' one by one, and choose the class with maximum probability.
 * P(Ci|'docToClassify') = P(Ci|W0) * ... * P(Ci|Wn), W0...Wn are words in document 'docToClassify'.
 *
 * P(Ci|Wi) = P(Wi|Ci) * P(Ci) / P(Wi).
 * P(Wi|Ci) = (number of documents belonging to class 'Ci' containing word 'Wi') / (number of words in documents belonging to class 'Ci').
 * P(Ci) = (number of words in documents belonging to class 'Ci') / (number of words in all documents of all classes).
 * P(Wi) = (number of documents of all classes containing word 'Wi') / (number of words in all documents of all classes).
 *
 * N.B. Above we plus 1 to the nominator when word 'Wi' appears in some document no matter how many times of occurrences of it in the 
 * document when calculating P(Wi|Ci) and P(Wi). 
 * We can take the occurrences of word 'Wi' in some document into account, which induces the following formulas:
 * P(Wi|Ci) = (number of occurrences of word 'Wi' in documents belonging to class 'Ci') / (number of words in documents belonging to class 'Ci').
 * P(Wi) = (number of occurrences of word 'Wi' in documents of all classes) / (number of words in all documents of all classes).
 */ 
int OfflineNaiveBayesClassifier::classify(const std::string& docToClassify) {
	float* cateProb = new float[categoryNum];
	for(int j = 0; j < categoryNum; ++j)
		cateProb[j] = 1.0;

	int totalTokens = 0;
	for(int i = 0; i < categoryNum; ++i) {
		std::map<std::string, int>& cateTokens = documentTokens[i];
		for(std::map<std::string, int>::iterator iter = cateTokens.begin(); iter != cateTokens.end(); ++iter)
			totalTokens += (*iter).second;
	}
#ifdef NBC_DEBUG
	std::cout<<"totalTokens = "<<totalTokens<<std::endl;
#endif

	int* cateFreq = new int[categoryNum];
	int totalFreq;
	// cateTokens[i] is the number of tokens in the documents belong to category 'i'.
	int* cateTokens = new int[categoryNum];
	FileTokenizer tokenizer(docToClassify);
	while(tokenizer.hasNext()) {
		for(int j = 0; j < categoryNum; ++j)
			cateFreq[j] = 0;
		totalFreq = 0;
		for(int j = 0; j < categoryNum; ++j)
			cateTokens[j] = 0;

		std::string token = tokenizer.nextToken();
#ifdef NBC_DEBUG
		std::cout<<"token = "<<token<<std::endl;
#endif
		for(int i = 0; i < categoryNum; ++i) {
			std::vector<std::string>& cateDocuments = documents[i];
			for(std::vector<std::string>::const_iterator iter = cateDocuments.begin(); iter != cateDocuments.end(); ++iter) {
				std::string docName = *iter;
				if(documentVectors[i][docName][vocabulary[token]] == 1) {
					++cateFreq[i];
					++totalFreq;
				}
				cateTokens[i] += documentTokens[i][docName];
			}
		}

		for(int i = 0; i < categoryNum; ++i) {
			float prob;
			if(cateFreq[i] == 0)
				prob = static_cast<float>(cateTokens[i]) / totalTokens;
			else
				prob = (static_cast<float>(cateFreq[i]) / cateTokens[i]) * (static_cast<float>(cateTokens[i]) / totalTokens) / 
					(static_cast<float>(totalFreq) / totalTokens);
			cateProb[i] *= prob;
	
#ifdef NBC_DEBUG
			std::cout<<"cateFreq["<<i<<"] = "<<cateFreq[i]<<", cateTokens["<<i<<"] = "<<cateTokens[i]<<", prob = "<<prob<<
				", cateProb["<<i<<"] = "<<cateProb[i]<<std::endl;
#endif
		}
	}
	delete[] cateFreq;
	delete[] cateTokens;

	int category = 0;
	float maxProb = cateProb[0];
	for(int i = 1; i < categoryNum; ++i) {
		if(maxProb < cateProb[i]) {
			category = i;
			maxProb = cateProb[i];
		}
	}
#ifdef NBC_DEBUG
	std::cout<<"maxProb = "<<maxProb<<", category = "<<category<<std::endl;
#endif
	delete[] cateProb;

	return category;
}

OfflineNaiveBayesClassifier::~OfflineNaiveBayesClassifier() {
	if(categoryNum > 0) {
		delete[] documents;

#ifndef USE_VECTOR		
		for(int category = 0; category < categoryNum; ++category) {
			std::map<std::string, int*>& cateVectors = documentVectors[category];
			for(std::map<std::string, int*>::iterator iter = cateVectors.begin(); iter != cateVectors.end(); ++iter) {
				int* value = (*iter).second;
				delete[] value;
			}
		}
#endif
		delete[] documentVectors;
		delete[] documentTokens;
	}
}
