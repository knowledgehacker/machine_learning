/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "Apriori.h"
#include <iostream>

template<typename U>
std::ostream& operator<<(std::ostream& os, const SampleSet<U>& ss) {
	os<<"["<<ss.support<<", {";
	int sampleSetSize = ss.size();
	int i;
	for(i = 0; i < sampleSetSize-1; ++i)
		os<<ss.sampleSet[i]<<", ";
	os<<ss.sampleSet[i]<<"}]";

	return os;
}

template<typename U>
bool operator<(const SampleSet<U>& ss1, const SampleSet<U>& ss2) {
	return ss1.sampleSet < ss2.sampleSet;
}

/*
 * SampleSet implmentation.
 */
template<typename T>
SampleSet<T>::SampleSet(const std::vector<T>& set, const float sup):
	sampleSet(set), support(sup) {
}

template<typename T>
int SampleSet<T>::size()const {
	return sampleSet.size();
}

template<typename T>
void SampleSet<T>::add(const T& sample) {
	sampleSet.push_back(sample);
}

template<typename T>
T& SampleSet<T>::operator[](const int index) {
	return sampleSet[index];
}

template<typename T>
const T& SampleSet<T>::operator[](const int index)const {
	return sampleSet[index];
}

template<typename T>
void SampleSet<T>::setSupport(const float sup) {
	support = sup;
}

template<typename T>
float SampleSet<T>::getSupport()const {
	return support;
}

template<typename T>
bool SampleSet<T>::compareTo(const SampleSet<T>& ss, const int k)const {
	if((ss.size() != size()) || (size() <= k))
		return false;

	for(int i = 0; i < k; ++i)
		if(sampleSet[i] != ss.sampleSet[i])
			return false;

	return true;
}

template<typename T>
SampleSet<T>::~SampleSet() {
}

/*
 * Apriori implementation.
 */
template<typename DataType>
Apriori<DataType>::Apriori(const std::vector<std::set<DataType> >& data, const float thres):
	samples(data), rowNum(data.size()), 
	threshold(thres), sampleSetsVector(rowNum) {
	// Retrieve all unique samples from 'samples'
	for(int i = 0; i < rowNum; ++i){
		const std::set<DataType>& rowSamples = samples[i];
		for(typename std::set<DataType>::const_iterator iter = rowSamples.begin();
			iter != rowSamples.end(); ++iter)
			uniqueSamples.insert(*iter);
	}

	genFirstSampleSet();
	
	int uniqueSampleNum = uniqueSamples.size();
#ifdef AP_DBG
	for(typename std::set<DataType>::const_iterator iter = uniqueSamples.begin();
		iter != uniqueSamples.end(); ++iter)
		std::cout<<*iter<<" ";
	std::cout<<std::endl;
#endif
	int k = 0;
	while((sampleSetsVector[k].size() > 0) && (k < uniqueSampleNum))
		genNextSampleSet(k++);
}

/*
 * Generate initial SampleSets that have only one sample.
 * We calculate the frequency of each sample in 'uniqueSamples' and create a SampleSet for 
 * the sample if its frequency >= 'threshold'. Add the SampleSets to 'sampleSetsVector[0]'.
 */
template<typename DataType>
void Apriori<DataType>::genFirstSampleSet() {
	std::set<SampleSet<DataType> > sampleSets;
	for(typename std::set<DataType>::const_iterator iter = uniqueSamples.begin();
		iter != uniqueSamples.end(); ++iter) {
		DataType uniqueSample = *iter;

		int freq = 0;
		for(int i = 0; i < rowNum; ++i) {
			const std::set<DataType>& rowSamples = samples[i];
			if(rowSamples.find(uniqueSample) != rowSamples.end())
				++freq;
		}
		float support = static_cast<float>(freq) / rowNum;
		if(support >= threshold) {
			std::vector<DataType> oneSample(1);
			oneSample[0] = uniqueSample;
			SampleSet<DataType> sampleSet(oneSample, support);

			/*
			 * Note if we do not allocate space for a std::vector initially, 
			 * adding to it by index will cause segment fault(index out of bound).
			 * But adding to it by push_back is OK, since push_back causes reallocate
			 * if the space is not enough.
			 */
			sampleSetsVector[0].insert(sampleSet);
		}
	}
}

/*
 * Generate SampleSets that have k+2 samples from SampleSets that have k+1 samples
 * (SampleSets in 'sampleSetsVector[k]').
 * Firstly, we calculate all the non-duplicated possible SampleSets that have k+2
 * samples from 'SampleSetsVector[k]', put it in 'newSampleSets'.
 * Then, we iterate 'newSampleSets' to calculate the frequency of each SampleSet,
 * and insert the ones whose frequency >= threshold into 'sampleSetsVector[k+1]'.
 */
template<typename DataType>
void Apriori<DataType>::genNextSampleSet(const int k) {
	std::set<SampleSet<DataType> > newSampleSets;

	// note sampleSet in sampleSetsVector[k] has k+1 samples
	const std::set<SampleSet<DataType> >& sampleSets = sampleSetsVector[k];
	typedef typename std::set<SampleSet<DataType> >::iterator SampleSetIterator;
	SampleSetIterator curr = sampleSets.begin();
	while(curr != sampleSets.end()) {
		SampleSetIterator tmp = curr;
		for(SampleSetIterator next = ++curr;
			next != sampleSets.end(); ++next) {
			if((*tmp).compareTo(*next, k)) {
				SampleSet<DataType> newSampleSet(*tmp);
				newSampleSet.add((*next)[k]);
				newSampleSets.insert(newSampleSet);
			}
		}
	}

	for(SampleSetIterator iter = newSampleSets.begin();
		iter != newSampleSets.end(); ++iter) {
		const SampleSet<DataType>& newSampleSet = (*iter);
		int newSampleSetSize = newSampleSet.size();

		int freq = 0;
		for(int i = 0; i < rowNum; ++i) {
			const std::set<DataType>& rowSamples = samples[i];
			bool exists = true;
			for(int j = 0; j < newSampleSetSize; ++j)
				if(rowSamples.find(newSampleSet[j]) == rowSamples.end()) {
					exists = false;
					break;
				}

			if(exists == true)
				++freq;
		}
		float support = static_cast<float>(freq) / rowNum;
		if(support >= threshold) {
			SampleSet<DataType> tmp(*iter);
			tmp.setSupport(support);
			
			sampleSetsVector[k+1].insert(tmp);
		}
	}
}

template<typename DataType>
void Apriori<DataType>::print()const {
	int sampleSetsNum = sampleSetsVector.size();
	for(int i = 0; i < sampleSetsNum; ++i) {
		std::cout<<i+1<<"- ";
		const std::set<SampleSet<DataType> >& sampleSets = sampleSetsVector[i];
		for(typename std::set<SampleSet<DataType> >::const_iterator iter = 
			sampleSets.begin(); iter != sampleSets.end(); ++iter)
			std::cout<<*iter<<" ";
		std::cout<<std::endl;
	}
}

template<typename DataType>
Apriori<DataType>::~Apriori() {
}
