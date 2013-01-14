#ifndef _OFFLINE_K_MEANS_H_
#define _OFFLINE_K_MEANS_H_

#include "Samples.h"
#include <set>
#include <utility>

class OfflineKMeans {
private:
	const Samples<float>& samples;
	int featureNum;
	int sampleNum;
	std::pair<float*, std::set<float*> >* clusters;
	int clusterNum;
	// Mapping from samples to clusters. clusterIndexes[i] means sample indexed by 'i' is in cluster indexed by 'clusterIndexes[i]'.
	int* clusterIndexes;

public:
	OfflineKMeans(const Samples<float>& data, const int k);
	void print()const;
	~OfflineKMeans();

private:
	void initClusters();
	bool reCluster();
	void updateCentroids();
	int cluster(const float* sample, const int len)const;
	float distance(const float* sample1, const float* sample2, const int len)const;
};

#endif
