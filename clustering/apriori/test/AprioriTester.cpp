/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "Apriori.cpp"
#include <string>

int main(int argc, char** argv) {
	/*
	 * 1- [0.4, {Item0}] [0.4, {Item1}] [0.8, {Item2}] 
	 *	  	[0.6, {Item3}] [0.4, {Item4}] [0.6, {Item5}] 
	 * 2- [0.4, {Item0, Item2}] [0.4, {Item1, Item2}] 
	 * 		[0.4, {Item2, Item3}] [0.6, {Item2, Item5}]
	 * 		[0.4, {Item3, Item5}] 
	 * 3- [0.4, {Item2, Item3, Item5}]
	 */
	std::string transactions[5][4] = {
		{"Item0", "Item2", "Item3", "Item5"},
		{"Item1", "Item2", "Item4", "Item5"},
		{"Item2", "Item3", "Item5", ""},
		{"Item0", "Item1", "Item2", ""},
		{"Item3", "Item4", ""	  , ""}
	};
	std::vector<std::set<std::string> > data;
	for(int i = 0; i < 5; ++i) {
		std::set<std::string> line;
		for(int j = 0; j < 4; ++j) {
			if(transactions[i][j] != "")
				line.insert(transactions[i][j]);
		}
		data.push_back(line);
	}
	float threshold = 0.4;
	Apriori<std::string> apriori(data, 0.4);
	apriori.print();

	return 0;
}
