/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "DecisionTree.h"
#include "Utils.h"
#include "Samples.cpp"
#include <cmath>
#include <map>
#include <queue>
#include <iostream>

DecisionTree::DecisionTree(const Samples<std::string>& samples): root(0), trainingSamples(samples),
	featureNum(samples.getFeatureNum()), sampleNum(samples.getSampleNum()) {
	std::vector<int> indexes(sampleNum);
	for(int i = 0; i < sampleNum; ++i)
		indexes[i] = i;

	std::list<int> nonSplitColumns;
	for(int i = 0; i < featureNum; ++i)
		nonSplitColumns.push_back(i);
	
	build(root, "", indexes, nonSplitColumns);
}

Node* DecisionTree::getRoot()const {
	return root;
}

bool DecisionTree::inSameCategory(const std::vector<int>& indexes)const {
	const std::string* categories = trainingSamples.getCategories();
	std::vector<int>::const_iterator iter = indexes.begin(); 
	std::string category = categories[*iter];
	while(++iter != indexes.end())
		if(categories[*iter] != category)
			return false;

	return true;
}

/*
 * Splits items indexed by 'indexes', and create an InnerNode for the feature on which the split based.
 * Add the InnerNode as a link to 'parent', and builds links for the InnerNode recursively.
 * When we run out of the features to split, or the items indexed by 'indexes' are in the same class, 
 * a LeftNode is created and function 'build' returns.
 */
void DecisionTree::build(InnerNode*& parent, const std::string& policyOrValue, 
	const std::vector<int>& indexes, std::list<int>& nonSplitColumns) {
	// if no feature to split any more, create a left node and add the items indexed by 'indexes' to it.
	if((nonSplitColumns.empty() == true) || (inSameCategory(indexes) == true)) {
		LeftNode* leftNode = new LeftNode(policyOrValue);
		for(std::vector<int>::const_iterator indexIter = indexes.begin(); 
			indexIter != indexes.end(); ++indexIter)
			leftNode->addItem(trainingSamples.getSample(*indexIter));
		
		parent->addLink(leftNode);

		return;
	}

	int splitColumn;
	std::map<std::string, std::vector<int> > splits;
	splitItems(indexes, nonSplitColumns, splits, splitColumn);

	// create an inner node for the feature in 'splitColumn'
	InnerNode* innerNode = 0;
	if(parent == 0)
		innerNode = parent = new InnerNode(trainingSamples.getFeatureName(splitColumn));
	else
		parent->addLink(innerNode = new InnerNode(policyOrValue + ": " + 
			trainingSamples.getFeatureName(splitColumn)));
			
	std::map<std::string, std::vector<int> >::iterator iter = splits.begin();
	while(iter != splits.end()) {
		std::list<int> subNonSplitColumns(nonSplitColumns);
		build(innerNode, (*iter).first, (*iter).second, subNonSplitColumns);
	
		++iter;
	}
}

/*
 * Calculates entropies based on features in 'nonSplitColumns' and 
 * find the feature on which we will get the maximum entropy bases.
 * Split the items indexed by 'indexes' based on the feature found, 
 * store the feature index in 'splitColumn' and splits' indexes in splits.
 *
 * [in] nonSplitColumns: input parameter, the columns hasn't been split.
 * [in] indexes: indexes of items in 'items' to split in this round.
 * [out] splitColumn: output parameter, the column on which the items will be split based in this round.
 * [out] splits: output parameter, the <feature value, item list> pairs after split in this round.
 */
void DecisionTree::splitItems(const std::vector<int>& indexes, std::list<int>& nonSplitColumns, 
	std::map<std::string, std::vector<int> >& splits, int& splitColumn) {
	std::list<int>::iterator iter = nonSplitColumns.begin();
	double maxEntropy = calcEntropy(*iter);
	std::list<int>::iterator maxCol = iter;

	for(; iter != nonSplitColumns.end(); ++iter) {
		double entropy = calcEntropy(*iter);
		if(maxEntropy < entropy) {
			maxEntropy = entropy;
			maxCol = iter;
		}
	}
	nonSplitColumns.erase(maxCol);
	splitColumn = *maxCol;

	// split based on feature in 'splitColumn'
	for(std::vector<int>::const_iterator indexIter = indexes.begin(); 
		indexIter != indexes.end(); ++indexIter)
		splits[trainingSamples.getSample(*indexIter)[splitColumn]].push_back(*indexIter);
}

float DecisionTree::calcBaseEntropy()const {
	return calcEntropy(featureNum-1);
}

float DecisionTree::calcEntropy(const int col)const {
	std::map<std::string, int> valueFrequency;

	for(int row = 0; row < sampleNum; ++row)
		++valueFrequency[trainingSamples.getSample(row)[col]];
		
	float entropy = 0.0;
	for(std::map<std::string, int>::iterator iter = valueFrequency.begin();
		iter != valueFrequency.end(); ++iter) {
		float probability = (*iter).second/static_cast<float>(sampleNum);
		entropy += probability * logN(probability, 2);
	}
	
	return -entropy;
}

void DecisionTree::breadthFirstTraverse()const {
	if(root == 0)
		return;

	std::vector<int> nodeNumPerLevel(10);
	++nodeNumPerLevel[0];
	std::queue<Node*> nodes;
	nodes.push(root);

	int level = 0;
	int nodeNumVisited = 0;
	while(nodes.empty() == false) {
		Node* node = nodes.front();
		nodes.pop();
		++nodeNumVisited;
		
		InnerNode* innerNode = dynamic_cast<InnerNode*>(node);
		if(innerNode != 0) {
			std::cout<<innerNode->getPolicy()<<" ";
			
			for(InnerNode::iterator iter = innerNode->begin();
				iter != innerNode->end(); ++iter){
				nodes.push(*iter);
				++nodeNumPerLevel[level+1];
			}
		}else {
			LeftNode* leftNode = dynamic_cast<LeftNode*>(node);
			if(leftNode != 0) {
				std::cout<<leftNode->getValue()<<": ";
				std::cout<<"[";
				for(LeftNode::iterator iter = leftNode->begin(); 
					iter != leftNode->end(); ++iter) {
					const std::string* item = *iter;
					std::cout<<"(";
					int i;
					for(i = 0; i < featureNum; ++i)
						std::cout<<item[i]<<", ";
					std::cout<<item[i]<<") ";
				}
				std::cout<<"]"<<" ";
			} else {
				std::cout<<"Invalid node type!"<<std::endl;
			}
		}
		if(nodeNumPerLevel[level] == nodeNumVisited) {
			nodeNumVisited -= nodeNumPerLevel[level];
			++level;
			std::cout<<std::endl;
		}
	}
}

void DecisionTree::destroy(Node* node) {
	if(node == 0)
		return;

	InnerNode* innerNode = dynamic_cast<InnerNode*>(node);
	if(innerNode != 0) {
		InnerNode::iterator iter = innerNode->begin();
		while(iter != innerNode->end()) {
			destroy(*iter);
			++iter;
		}
	}

	delete node;
}

DecisionTree::~DecisionTree() {
	destroy(root);
}
