#include "DecisionTree.h"
#include "Utils.h"
#include <cmath>
#include <map>
#include <queue>
#include <iostream>

DecisionTree::DecisionTree(const InputDataSet& inputDataSet): root(0), inputData(inputDataSet), itemNum(inputDataSet.getItemNum()) {
	std::vector<int> indexes(itemNum);
	for(int i = 0; i < itemNum; ++i)
		indexes[i] = i;

	std::list<int> nonSplitColumns;
	int fn = Item::featureNum;
	for(int i = 0; i < fn-1; ++i)
		nonSplitColumns.push_back(i);
	
	build(root, "", indexes, nonSplitColumns);
}

Node* DecisionTree::getRoot()const {
	return root;
}

bool DecisionTree::inSameClass(const std::vector<int>& indexes)const {
	/*
	std::vector<int>::const_iterator iter = indexes.begin();
	std::string value = inputData[*(iter++)].getLabel();
	while((iter != indexes.end()) && (inputData[*(iter++)].getLabel() == value));
	if(iter == indexes.end())
		return true;
	*/
	int indexNum = indexes.size();
	if(indexNum > 0) {
		std::string value = inputData[indexes[0]].getLabel();
		int i;
		for(i = 1; i < indexNum; ++i)
			if(inputData[indexes[i]].getLabel() != value)
				break;
		if(i == indexNum)
			return true;
	}

	return false;
}

/*
 * Splits items indexed by 'indexes', and create an InnerNode for the feature on which the split based.
 * Add the InnerNode as a link to 'parent', and builds links for the InnerNode recursively.
 * When we run out of the features to split, or the items indexed by 'indexes' are in the same class(have the same label), 
 * a LeftNode is created and function 'build' returns.
 */
void DecisionTree::build(InnerNode*& parent, const std::string& policyOrValue, const std::vector<int>& indexes, std::list<int>& nonSplitColumns) {
	// if no feature to split any more, then create a left node and add the items indexed by 'indexes' to the left node
	if((nonSplitColumns.empty() == true) || (inSameClass(indexes) == true)) {
		LeftNode* leftNode = new LeftNode(policyOrValue);
		for(std::vector<int>::const_iterator indexIter = indexes.begin(); indexIter != indexes.end(); ++indexIter)
			leftNode->addItem(inputData[*indexIter]);
		
		parent->addLink(leftNode);

		return;
	}

	int splitColumn;
	std::map<std::string, std::vector<int> > splits;
	splitItems(indexes, nonSplitColumns, splits, splitColumn);

	// create an inner node for the feature in 'splitColumn'
	InnerNode* innerNode = 0;
	if(parent == 0)
		innerNode = parent = new InnerNode(inputData.getFeature(splitColumn));
	else
		parent->addLink(innerNode = new InnerNode(policyOrValue + ": " + inputData.getFeature(splitColumn)));
	std::map<std::string, std::vector<int> >::iterator iter = splits.begin();
	while(iter != splits.end()) {
		std::list<int> subNonSplitColumns(nonSplitColumns);
		build(innerNode, (*iter).first, (*iter).second, subNonSplitColumns);
	
		++iter;
	}
}

/*
 * Calculates entropies based on features in 'nonSplitColumns' and find the feature on which we will get the maximum entropy bases.
 * Split the items indexed by 'indexes' based on the feature found, store the feature index in 'splitColumn' and splits' indexes in splits.
 *
 * [in] nonSplitColumns: input parameter, the columns hasn't been split.
 * [in] indexes: indexes of items in 'items' to split in this round.
 * [out] splitColumn: output parameter, the column on which the items will be split based in this round.
 * [out] splits: output parameter, the <feature value, item list> pairs after split in this round.
 */
void DecisionTree::splitItems(const std::vector<int>& indexes, std::list<int>& nonSplitColumns, std::map<std::string, std::vector<int> >& splits, 
	int& splitColumn) {
	std::list<int>::iterator iter = nonSplitColumns.begin();
	double maxEntropy = calcEntropy(indexes, *iter);
	std::list<int>::iterator maxCol = iter;

	for(; iter != nonSplitColumns.end(); ++iter) {
		double entropy = calcEntropy(indexes, *iter);
		if(maxEntropy < entropy) {
			maxEntropy = entropy;
			maxCol = iter;
		}
	}
	nonSplitColumns.erase(maxCol);
	splitColumn = *maxCol;

	// split based on feature in 'splitColumn'
	for(std::vector<int>::const_iterator indexIter = indexes.begin(); indexIter != indexes.end(); ++indexIter)
		splits[inputData[*indexIter][splitColumn]].push_back(*indexIter);
}

float DecisionTree::calcBaseEntropy(const std::vector<int>& indexes)const {
	return calcEntropy(indexes, Item::featureNum-1);
}

float DecisionTree::calcEntropy(const std::vector<int>& indexes, const int col)const {
	std::map<std::string, int> valueFrequency;

	for(int row = 0; row < itemNum; ++row)
		++valueFrequency[inputData[row][col]];
		
	float entropy = 0.0;
	for(std::map<std::string, int>::iterator iter = valueFrequency.begin(); iter != valueFrequency.end(); ++iter) {
		float probability = (*iter).second/static_cast<float>(itemNum);
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
			
			for(InnerNode::iterator iter = innerNode->begin(); iter != innerNode->end(); ++iter){
				nodes.push(*iter);
				++nodeNumPerLevel[level+1];
			}
		}else {
			LeftNode* leftNode = dynamic_cast<LeftNode*>(node);
			if(leftNode != 0) {
				std::cout<<leftNode->getValue()<<": ";
				std::cout<<"[";
				for(LeftNode::iterator iter = leftNode->begin(); iter != leftNode->end(); ++iter)
					std::cout<<*iter<<" ";
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
