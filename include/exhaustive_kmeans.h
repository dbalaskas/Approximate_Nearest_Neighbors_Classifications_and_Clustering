#ifndef EXHAUSTIVE_KMEANS_H
#define EXHAUSTIVE_KMEANS_H

#include "./numc.h"
#include "./prediction_results.h"
#include "./exhaustive_knn.h"
#include <vector>
#include <numeric>
#include <random>
#include <queue>

template <typename NumCDataType> 
class ExhaustiveKmeans {
    private:
        NumC<NumCDataType>* data;
        NumC<NumCDataType>* centroids;
        NumC<NumCDataType>* centroidsMap;
        ExhaustiveKnn<NumCDataType>* knnEstimator;
        std::vector<NumCDistType> silhouette;
        NumCDistType overallSilhouette;
        int numOfClusters;
        int numOfDimensions;
        int numOfPoints;
        NumCDistType calculateSilhouette(NumCDistType distA, NumCDistType distB);
        void getSilhouette(Results* results);
        void randomInit();
        void kmeansInit();

    public:
        ExhaustiveKmeans(int numOfClusters);
        ExhaustiveKmeans(NumC<NumCDataType>* data, int numOfClusters);
        ~ExhaustiveKmeans();

        void fit(NumC<NumCDataType>* trainData);
        void transform();
        // Results* predict_knn(Vector<NumCDataType> vector);
        // Results* predict_knn(NumC<NumCDataType>* testData);
        Results* get_centroids();
        
};

template class ExhaustiveKmeans<int>;
template class ExhaustiveKmeans<long>;
template class ExhaustiveKmeans<double>;

#endif