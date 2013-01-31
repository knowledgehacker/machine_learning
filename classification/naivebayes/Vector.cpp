/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "Vector.h"
#include <iostream>

template<typename DataType>
Vector<DataType>::Vector(): data(0), length(0) {
#ifdef NBC_DEBUG
	std::cout<<"Vector no-argument constructor is called!"<<std::endl;	
#endif
}

template<typename DataType>
Vector<DataType>::Vector(const int len): length(len) {
#ifdef NBC_DEBUG
	std::cout<<"Vector one-argument constructor is called!"<<std::endl;
#endif

	if(length == 0)
		data = 0;
	else
		data = new DataType[length];
}

template<typename DataType>
Vector<DataType>::Vector(const Vector<DataType>& vec) {
#ifdef NBC_DEBUG
	std::cout<<"Vector copy constructor is called!"<<std::endl;
#endif

	length = vec.length;
	if(length == 0)
		data = 0;
	else {
		data = new DataType[length];
		for(int i = 0; i < length; ++i)
			data[i] = vec.data[i];
	}
}

template<typename DataType>
Vector<DataType>& Vector<DataType>::operator=(const Vector<DataType>& vec) {
	std::cout<<"Vector assignment operator= is called!"<<std::endl;

	if(this != &vec) {
		length = vec.length;
		if(length == 0)
			data = 0;
		else {
			if(data != 0)
				delete[] data;
			data = new DataType[length];
			for(int i = 0; i < length; ++i)
				data[i] = vec.data[i];
		}
	}

	return *this;
}

template<typename DataType>
DataType& Vector<DataType>::operator[](const int index)throw(IndexOutOfBound) {
	if(index >=  length)
		throw IndexOutOfBound(index);

	return data[index];
}

template<typename DataType>
Vector<DataType>::~Vector() {
	if(data != 0)
		delete[] data;
}
