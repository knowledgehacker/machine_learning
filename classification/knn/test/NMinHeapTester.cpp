#include "NMinHeap.cpp"
#include <iostream>

int main(int argc, char** argv) {
	NMinHeap<double, int> nmh(5);
	std::pair<double, int> elems[] = {
		std::make_pair(3.0, 2), std::make_pair(3.3, 1), std::make_pair(1.2, 3),
		std::make_pair(2.5, 4), std::make_pair(4.2, 3), std::make_pair(3.0, 5), 
		std::make_pair(2.0, 4), std::make_pair(1.8, 5), std::make_pair(4.5, 2), 
		std::make_pair(5.8, 5)
	};

	for(int i = 0; i < 10; ++i)
		nmh.insert(elems[i].first, elems[i].second);
	int size = nmh.size();
	for(int i = 0; i < size; ++i)
		std::cout<<"("<<nmh[i].first<<", "<<nmh[i].second<<")"<<std::endl;

	return 0;
}
