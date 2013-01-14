#ifndef _DECISION_TREE_H_
#define _DECISION_TREE_H_

#include "InputDataSet.h"
#include "Node.h"
#include <vector>
#include <list>
#include <map>

/*
 * TODO: write a general serialize/deserialize framework in C++, and use it to store the decision tree constructed to disk for later use.
 */
class DecisionTree {
private:
	/*
	 * Cached all the items in memory will not work when the amount of items is very huge.
	 * And every time calling member functions needs to copy all the times in the vector, it is very inefficient.
	 * So here 'items' is a reference to the items retrieved from data file by InputDataSet.
	 * And we passed the index in 'items' in member functions instead of Item itself.
	 */
	const InputDataSet& inputData;
	int itemNum;
	InnerNode* root;

	std::list<int> splitColumns;

public:
	DecisionTree(const InputDataSet& inputDataSet);
	Node* getRoot()const;
	void breadthFirstTraverse()const;
	~DecisionTree();

private:
	void build(InnerNode*& parent, const std::string& policyOrValue, const std::vector<int>& indexes, std::list<int>& nonSplitColumns);
	bool inSameClass(const std::vector<int>& indexes)const;
	void splitItems(const std::vector<int>& indexes, std::list<int>& nonSplitColumns, std::map<std::string, std::vector<int> >& splits, 
		int& splitColumn);
	float calcBaseEntropy(const std::vector<int>& indexes)const;
	float calcEntropy(const std::vector<int>& indexes, const int col)const;

	void destroy(Node* node);
};

#endif
