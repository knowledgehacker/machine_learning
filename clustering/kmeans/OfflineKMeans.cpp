#include "OfflineKMeans.h"
#include "Samples.cpp"
#include <limits>
#include <cstdlib>
#include <iostream>

OfflineKMeans::OfflineKMeans(const Samples<float>& data, const int k): samples(data), featureNum(data.getFeatureNum()), 
	sampleNum(data.getSampleNum()), clusterNum(k) {
	initClusters();
#ifdef OKM_DBG
	print();
	while(reCluster() == true) {
		print();
	}
#else
	while(reCluster() == true);
#endif
}

void OfflineKMeans::initClusters() {
	// calculate the minimum and maximum values of each feature
	std::pair<float, float>* featureRanges = new std::pair<float, float>[featureNum];
	for(int j = 0; j < featureNum; ++j) {
		float minFeatureJ = std::numeric_limits<float>::max();
		float maxFeatureJ = std::numeric_limits<float>::min();
		for(int i = 0; i < sampleNum; ++i) {
			float feature = samples.getSample(i)[j];
			if(minFeatureJ > feature)
				minFeatureJ = feature;
			if(maxFeatureJ < feature)
				maxFeatureJ = feature;
		}

		featureRanges[j].first = minFeatureJ;
		featureRanges[j].second = maxFeatureJ;
	}

	// initialize centroids of clusters, feature values of each centroids are within the corresponding ones.
	clusters = new std::pair<float*, std::set<float*> >[clusterNum];
	for(int i = 0; i < clusterNum; ++i) {
		float* centroid = new float[featureNum];
		srand(i+1);
		float scale = static_cast<float>(rand()) / RAND_MAX;
		for(int j = 0; j < featureNum; ++j) {
			float minFeatureJ = featureRanges[j].first;
			float maxFeatureJ = featureRanges[j].second;
			centroid[j] = minFeatureJ + (maxFeatureJ - minFeatureJ) * scale;
		}
		clusters[i].first = centroid;
	}
	delete[] featureRanges;
#ifdef OKM_DBG
	std::cout<<"Centroids of clusters initialized randomly:"<<std::endl;
	for(int i = 0; i < clusterNum; ++i) {
		std::cout<<"<";
		float* centroid = clusters[i].first;
		int j;
		for(j = 0; j < featureNum-1; ++j)
			std::cout<<centroid[j]<<" ";
		std::cout<<centroid[j]<<">"<<std::endl;
	}
#endif

	// cluster all samples for the first time, setup mapping from samples to clusters and centroids of clusters.
	clusterIndexes = new int[sampleNum];
	for(int i = 0; i < sampleNum; ++i) {
		const float* sample = samples.getSample(i);

		int targetCluster = cluster(sample, featureNum);
		clusterIndexes[i] = targetCluster;
		clusters[targetCluster].second.insert(const_cast<float*>(sample));
	}
	updateCentroids();
}

/*
 * Cluster all the samples again.
 */
bool OfflineKMeans::reCluster() {
	bool clusterChanged = false;

    for(int i = 0; i < sampleNum; ++i) {
		const float* sample = samples.getSample(i);

		int oldCluster = clusterIndexes[i];
		int newCluster = cluster(sample, featureNum);
		if(newCluster != oldCluster) {
			clusterChanged = true;

#ifdef OKM_DBG
			std::cout<<"i = "<<i<<", oldCluster = "<<oldCluster<<", newCluster = "<<newCluster<<std::endl;
#endif
			// move sample from the old cluster to the new cluster.
			clusters[oldCluster].second.erase(const_cast<float*>(sample));
			clusters[newCluster].second.insert(const_cast<float*>(sample));

			// set clusterIndexes[i] to newCluster to indicate now sample indexed by 'i' is in cluster indexed by 'newCluster'.
			clusterIndexes[i] = newCluster;
		}
	}
	if(clusterChanged == true)
		updateCentroids();

	return clusterChanged;
}

/*
 * Update the centroids of all clusters, it is always called after all samples are clustered in one round.
 * To improve: only update the centroids of the affected clusters.
 */
void OfflineKMeans::updateCentroids() {
	for(int i = 0; i < clusterNum; ++i) {
		std::set<float*>& cluster = clusters[i].second;
		int clusterSize = cluster.size();
		if(clusterSize > 0) {
			for(int j = 0; j < featureNum; ++j) {
				float featureJ = 0.0;
				for(std::set<float*>::iterator iter = cluster.begin(); iter != cluster.end(); ++iter)
					featureJ += (*iter)[j];
				(clusters[i].first)[j] = featureJ / clusterSize;
			}
		}
	}
}

// find the cluster whose centroid is closest to sample
int OfflineKMeans::cluster(const float* sample, const int len)const {
	int targetCluster = -1;
	float minDist = std::numeric_limits<float>::max();
	for(int j = 0; j < clusterNum; ++j) {
		float dist = distance(sample, clusters[j].first, len);
		if(minDist > dist) {
			targetCluster = j;
			minDist = dist;
		}
	}

	return targetCluster;
}
	
float OfflineKMeans::distance(const float* sample1, const float* sample2, const int len)const {
	float dist = 0.0;
	for(int i = 0; i < len; ++i)
		dist += (sample1[i] - sample2[i]) * (sample1[i] - sample2[i]);

	return dist;
}

void OfflineKMeans::print()const {
	std::cout<<clusterNum<<" clusters:"<<std::endl;
	for(int i = 0; i < clusterNum; ++i) {
		float* centroid = clusters[i].first;
		std::cout<<i<<": <";
		int j;
		for(j = 0; j < featureNum-1; ++j)
			std::cout<<centroid[j]<<" ";
		std::cout<<centroid[j]<<">, ";

		std::set<float*>& clusterSamples = clusters[i].second;
		for(std::set<float*>::iterator iter = clusterSamples.begin(); iter != clusterSamples.end(); ++iter) {
			std::cout<<"[";
			for(j = 0; j < featureNum-1; ++j)
				std::cout<<(*iter)[j]<<" ";
			std::cout<<(*iter)[j]<<"] ";
		}
		std::cout<<std::endl;
	}
}

OfflineKMeans::~OfflineKMeans() {
	for(int i = 0; i < clusterNum; ++i)
		delete[] clusters[i].first;
	delete[] clusters;

	delete[] clusterIndexes;
}
