/*
 * Cluster a new sample, this is an online clustering algorithm.
 * Calculate the distances between the new sample and the centroids of clusters, the cluster with the minimum distance is the target cluster.
 * Add the new sample to the target cluster, add the mapping (new sample -> target cluster) to clusterIndexes, and update the centroid of the 
 * target cluster. But this is incorrect, since update of centroid of the target cluster may make the distances of other samples to it shorter, 
 * which needs recluster.
 * So to make the online k-means clustering algorithm works, we need to be able to add new samples to existing samples dynamically and recluster
 * all samples including the new and existing ones several times until no changes again.
 */
int OnlineKMeans::cluster(const float* sample, const int len) {
    int targetCluster = -1;
    float minDist = std::numeric_limits<float>::max();
    for(int j = 0; j < clusterNum; ++j) {
        float dist = distance(sample, clusters[j].first, len);
        if(minDistance > dist) {
            targetCluster = j;
            minDist = dist;
        }
    }       
            
    clusterIndexes.push_back(targetCluster);
    clusteers[targetCluster].second.insert(sample);

    // update the centroid of cluster indexed by 'targetCluster'
    for(int j = 0; j < featureNum; ++j) {
        int clusterSize = clusters[targetCluster].second.size();
        float oldCentroidFeatureJ = (clusters[targetCluster].first)[j];
        (clusters[targetCluster].first)[j] = (oldCentroidFeatureJ * (clusterSize-1) + sample[j]) / clusterSize;
    }

    return targetCluster;
}

