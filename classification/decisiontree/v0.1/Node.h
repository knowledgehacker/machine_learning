#ifndef _NODE_H_
#define _NODE_H_

#include "Item.h"
#include <string>
#include <vector>
#include <list>

class Node {
public:
	virtual ~Node() {
	}
};

class InnerNode: public Node {
private:
	std::string policy;
	std::list<Node*> links;
	int linkNum;

public:
	typedef std::list<Node*>::iterator iterator;

	InnerNode(std::string name);
	int getLinkNum()const;
	std::string getPolicy()const;
	/*
	 * "const Node* link" means "link is a pointer points to a const Node object", 
	 * while "Node *const link" means "a const pointer points to a Node object.
	 */
	void addLink(Node *const link);
	iterator begin();
	iterator end();	
	~InnerNode();
};

class LeftNode: public Node {
private:
	std::string value;
	std::vector<Item> items;
	int itemNum;

public:
	typedef std::vector<Item>::iterator iterator;

	LeftNode(const std::string& str);
	std::string getValue()const;
	int getItemNum()const;
	void addItem(const Item& item);
	iterator begin();
	iterator end();
	~LeftNode();
};

#endif
