#ifndef _APRIORI_H_
#define _APRIORI_H_

#include <vector>
#include <set>
#include <iostream>

template<typename T>
class SampleSet;

template<typename U>
std::ostream& operator<<(std::ostream& os, const SampleSet<U>& sampleSet);
template<typename U>
bool operator<(const SampleSet<U>& ss1, const SampleSet<U>& ss2);

template<typename T>
class SampleSet {
private:
	float support;
	std::vector<T> sampleSet;
	
public:
	SampleSet(const std::vector<T>& set, const float sup = 0.0);
	int size()const;
	void add(const T& sample);
	T& operator[](const int index);
	/*
	 * Note we should overload const version operator[], since when SampleSet is used with STL container, 
	 * the container ensures its elements should be const when they are used as r-value.
	 * It is a good habit to overload the const version as well when overload non-const operator[].
	 */
	const T& operator[](const int index)const;
	void setSupport(const float sup);
	float getSupport()const;
	friend std::ostream& operator<< <T> (std::ostream& os, const SampleSet<T>& sampleSet);
 	// Note we should overload operator< in SampleSet, since we use it as key of a std::set.
	friend bool operator< <T> (const SampleSet<T>& ss1, const SampleSet<T>& ss2);
	bool compareTo(const SampleSet<T>& ss, const int k)const;
	~SampleSet();
};

template<typename DataType>
class Apriori {
private:
	const std::vector<std::set<DataType> >& samples;
	int rowNum;
	float threshold;

	std::set<DataType> uniqueSamples;
	std::vector<std::set<SampleSet<DataType> > > sampleSetsVector;

public:
	Apriori(const std::vector<std::set<DataType> >& data, const float thres);
	void print()const;
	~Apriori();

private:
	void genFirstSampleSet();
	void genNextSampleSet(const int k);
	void filterSampleSet();
};

#endif
