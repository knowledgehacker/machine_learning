/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _N_MIN_HEAP_H_
#define _N_MIN_HEAP_H_

#include <utility>

/*
 * NMinHeap maintains N minimum elements with the maximum one among the N elements in root.
 */
template<typename K, typename V>
class NMinHeap {
private:
	std::pair<K, V>* pairs;
	int capacity;
	int num;

public:
	typedef std::pair<K, V>* iterator;

	NMinHeap(int n);
	bool insert(const K& k, const V& v);
	// iterate the heap like an array
	int size()const;
	std::pair<K, V> operator[](int index)const;
	~NMinHeap();

private:
	inline void swap(std::pair<K, V>* pair1, std::pair<K, V>* pair2);
};

#endif
