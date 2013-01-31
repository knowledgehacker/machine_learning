/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _OFFLINE_NAIVE_BAYES_CLASSIFIER_H_
#define _OFFLINE_NAIVE_BAYES_CLASSIFIER_H_

#include "InputDataSet.h"
#include "Vector.h"
#include <string>
#include <vector>
#include <map>

/*
 * We assume here the tokens in the document to classify have already been added to vocabulary, 
 * that is, this is an offline version.
 * Offline version is used to evaluate the classifier, in which case the training set and test set is fixed,
 * so the vocabulary is fixed.
 * While the practical scenario is always online, that is, the documents to classify come in real-time,
 * and in such a case, the vocabulary will be changed each time a new document is handled.
 */
class OfflineNaiveBayesClassifier {
private:
	std::vector<std::string>* documents;
	int categoryNum;
	std::map<std::string, int> vocabulary;
	int vocabularyNum;

#ifdef USE_VECTOR
	std::map<std::string, Vector<int> >* documentVectors;
#else
	std::map<std::string, int*>* documentVectors;
#endif
	std::map<std::string, int>* documentTokens;

public:
	OfflineNaiveBayesClassifier(InputDataSet& inputDataSet);
	int classify(const std::string& docToClassify);
	~OfflineNaiveBayesClassifier();
};

#endif
