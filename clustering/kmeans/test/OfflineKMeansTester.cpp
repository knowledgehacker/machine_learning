#include "OfflineKMeans.h"
#include "FloatDataReader.h"
#include "Samples.cpp"
#include <iostream>

int main(int argc, char** argv) {
	std::string dataFile("data/testSet.txt");
	//std::string dataFile("data/points.txt");
	Samples<float> samples(dataFile, FloatDataReader());
	/*
	int featureNum = samples.getFeatureNum();
	int sampleNum = samples.getSampleNum();
	for(int i = 0; i < sampleNum; ++i) {
		const float* sample = samples.getSample(i);
		for(int j = 0; j < featureNum; ++j)
			std::cout<<sample[j]<<" ";
		std::cout<<std::endl;
	}
	*/
	int k = 4;
	//int k = 3;
	OfflineKMeans okm(samples, k);
	std::cout<<"*****Result*****"<<std::endl;
	okm.print();

	return 0;
}
