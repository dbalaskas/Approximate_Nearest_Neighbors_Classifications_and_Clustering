#ifndef EXHAUSTIVE_KMEANS_H
#define EXHAUSTIVE_KMEANS_H

#include "./numc.h"
#include "./prediction_results.h"
#include "./exhaustive_knn.h"
#include <vector>
#include <numeric>
#include <random>
#include <queue>

enum ClusteringType {LLOYDS_CLUSTERING, LSH_CLUSTERING, HC_CLUSTERING};


template <typename NumCDataType> 
class ExhaustiveKmeans {
    private:
        NumC<NumCDataType>* data;
        NumC<NumCDataType>* centroids;
        std::vector<NumCDistType> silhouette;
        NumCDistType overallSilhouette;
        NumCDistType objectiveCost;
        int numOfClusters;
        int numOfDimensions;
        int numOfPoints;

        NumCDistType calculateSilhouette(NumCDistType distA, NumCDistType distB);
        void getSilhouette(Results* results);
        NumCDistType getObjectiveCost(Results* results);
        void randomInit();
        void kmeansInit();

        void medianCentroidsUpdate(Results* results);

        void transform_LLOYDS_CLUSTERING();
        void transform_LSH_CLUSTERING();
        void transform_HC_CLUSTERING();

    public:
        ExhaustiveKmeans(int numOfClusters);
        ExhaustiveKmeans(NumC<NumCDataType>* data, int numOfClusters);
        ~ExhaustiveKmeans();

        void fit(NumC<NumCDataType>* trainData);
        void transform(ClusteringType clusteringType);
        // Results* predict_knn(Vector<NumCDataType> vector);
        // Results* predict_knn(NumC<NumCDataType>* testData);
        NumC<NumCDataType>* getCentroids();
        std::vector<Results*>  getResults();
        NumCDistType getObjectiveCost();
        
};

template class ExhaustiveKmeans<int>;
template class ExhaustiveKmeans<long>;
template class ExhaustiveKmeans<double>;

#endif