#ifndef EXHAUSTIVE_KMEANS_H
#define EXHAUSTIVE_KMEANS_H

#include "./numc.h"
#include "./prediction_results.h"
#include "./exhaustive_knn.h"
#include <vector>
#include <numeric>
#include <random>
#include <queue>
#include <limits>

enum ClusteringType {LLOYDS_CLUSTERING, LSH_CLUSTERING, HC_CLUSTERING};


template <typename NumCDataType> 
class Kmedians {
    private:
        NumC<NumCDataType>* data;
        NumC<NumCDataType>* centroids;

        int numOfClusters;
        int numOfDimensions;
        int numOfPoints;
        int maxIterations;
        NumCDistType error;
        double transformTime;

        NumCDistType calculateSilhouette(NumCDistType distA, NumCDistType distB);
        std::vector<NumCDistType> getSilhouettes(Results* results);
        NumCDistType getObjectiveCost(Results* results);
        void medianCentroidsUpdate(Results* results);

        void randomInit();
        void kmeansInit();

        void transform_LLOYDS_CLUSTERING();
        void transform_LSH_CLUSTERING();
        void transform_HC_CLUSTERING();

    public:
        Kmedians(int numOfClusters=10, int maxIterations=100, NumCDistType error=1e-3);
        Kmedians(NumC<NumCDataType>* data, int numOfClusters, int maxIterations=100, NumCDistType error=1e-3);
        ~Kmedians();

        void fit(NumC<NumCDataType>* trainData);
        void transform(ClusteringType clusteringType);

        NumC<NumCDataType>* getCentroids();
        std::vector<Results*>  getResults();
        std::vector<NumCDistType> getSilhouettes();
        NumCDistType getObjectiveCost();
        
};

template class Kmedians<int>;
template class Kmedians<long>;
template class Kmedians<double>;

#endif