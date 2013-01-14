#include "Node.h"

// InnerNode implementation
InnerNode::InnerNode(std::string name): policy(name), linkNum(0) {
}

int InnerNode::getLinkNum()const {
	return linkNum;
}

std::string InnerNode::getPolicy()const {
	return policy;
}

void InnerNode::addLink(Node *const link) {
	links.push_back(link);
	++linkNum;
}

typename InnerNode::iterator InnerNode::begin() {
	return links.begin();
}

typename InnerNode::iterator InnerNode::end() {
	return links.end();
}

InnerNode::~InnerNode() {
}

// LeftNode implementation
LeftNode::LeftNode(const std::string& str): value(str), itemNum(0) {
}

std::string LeftNode::getValue()const {
	return value;
}

int LeftNode::getItemNum()const {
	return itemNum;
}

void LeftNode::addItem(const Item& item) {
	items.push_back(item);
	++itemNum;
}

typename LeftNode::iterator LeftNode::begin() {
	return items.begin();
}

typename LeftNode::iterator LeftNode::end() {
	return items.end();
}

LeftNode::~LeftNode() {
}

