#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "IndexOutOfBound.h"

template<typename DataType>
class Vector {
private:
	DataType* data;
	int length;

public:
	Vector();
	Vector(const int len);
	Vector(const Vector<DataType>& vec);
	Vector<DataType>& operator=(const Vector<DataType>& vec);
	DataType& operator[](const int index)throw(IndexOutOfBound);
	~Vector();
};

#endif
