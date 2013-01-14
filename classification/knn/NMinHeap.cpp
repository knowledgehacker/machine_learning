#include "NMinHeap.h"

template<typename K, typename V>
NMinHeap<K, V>::NMinHeap(int capacity) {
	this->capacity = capacity;
	this->num = 0;

	pairs = new std::pair<K, V>[capacity];
}

template<typename K, typename V>
void NMinHeap<K, V>::swap(std::pair<K, V>* pair1, std::pair<K, V>* pair2) {
	std::pair<K, V> tmp = *pair1;
	*pair1 = *pair2;
	*pair2 = tmp;
}

template<typename K, typename V>
bool NMinHeap<K, V>::insert(const K& k, const V& v) {
	if(num == capacity) {
		if(k > pairs[0].first)
			return false;

		// place (k, v) in pairs[0] and adjust it top-bottom
		pairs[0] = std::make_pair(k, v);
		int curr = 0;
		int child = 1; // 2*0+1
		while(child < capacity) {
			if((child+1 < capacity) && (pairs[child].first < pairs[child+1].first))
				child += 1;
			if(pairs[curr].first >= pairs[child].first)
				break;

			// swap pair in 'curr' and 'child'
			swap(&pairs[curr], &pairs[child]);

			curr = child;
			child = 2*curr+1;
		}
	}else {
		// place (k, v) in pairs[num] and adjust it bottom-up
		pairs[num] = std::make_pair(k, v);
		int curr = num;
		int parent = (curr-1)/2;
		while(curr > 0) {
			if(pairs[curr].first <= pairs[parent].first)
				break;

			// swap pair in 'curr' and 'parent'
			swap(&pairs[curr], &pairs[parent]);

			curr = parent;
			parent = (curr-1)/2;
		}

		++num;
	}

	return true;
}

template<typename K, typename V>
int NMinHeap<K, V>::size()const {
	return num;
}

template<typename K, typename V>
std::pair<K, V> NMinHeap<K, V>::operator[](int index)const {
	return pairs[index];
}

template<typename K, typename V>
NMinHeap<K, V>::~NMinHeap() {
	delete[] pairs;
}

