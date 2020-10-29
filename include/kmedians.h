#ifndef KMEDIANS_H
#define KMEDIANS_H

#include "./numc.h"
#include "./prediction_results.h"
#include "./exhaustive_knn.h"
#include "./hc_classifier.h"
#include "./lsh_classifier.h"
#include <vector>
#include <numeric>
#include <random>
#include <queue>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#define ERROR 1e-3
#define MAX_ITER 100

enum ClusteringType {LLOYDS_CLUSTERING, LSH_CLUSTERING, HC_CLUSTERING};

typedef struct ConfigurationData{
    int number_of_clusters; // K of K-medians
    int L;                  // default: L=3
    int k;                  // k of LSH for vectors, default: 4
    int M;                  // M of Hypercube, default: 10
    int d;                  // k of Hypercube, default: 3
    int probes;             // probes of Hypercube, default: 2
  
    ConfigurationData()
     :number_of_clusters{10}, 
     L{3}, 
     k{4}, 
     M{10},
     d{3}, 
     probes{2} {};

    ~ConfigurationData() {
        number_of_clusters = -1;
    };

    bool isEmpty() { return number_of_clusters == -1; };
    void print() {
        std::cout << "-------------------------" << std::endl;
        std::cout << "Configuration: " << std::endl;
        std::cout << "  number_of_clusters: " << number_of_clusters << std::endl;
        std::cout << "  L: " << L << std::endl;
        std::cout << "  k: " << k << std::endl;
        std::cout << "  M: " << M << std::endl;
        std::cout << "  d: " << d << std::endl;
        std::cout << "  probes: " << probes << std::endl;
        std::cout << "-------------------------" << std::endl;
    }

} ConfigurationData;

ConfigurationData readConfiguration(char* configurationFile);


template <typename NumCDataType> 
class Kmedians {
    private:
        NumC<NumCDataType>* data;
        NumC<NumCDataType>* centroids;
        ConfigurationData configurationData;

        int numOfClusters;
        int numOfDimensions;
        int numOfPoints;
        int maxIterations;
        NumCDistType error;
        double transformTime;

        // hc and lsh parameters
        int L;                  // default: L=3
        int k;                  // k of LSH for vectors, default: 4
        int M;                  // M of Hypercube, default: 10
        int d;                  // k of Hypercube, default: 3
        int probes;             // probes of Hypercube, default: 2

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
        Kmedians(ConfigurationData configurationData, int maxIterations=MAX_ITER, NumCDistType error=ERROR);
        // needs delete after configuration file
        Kmedians(int numOfClusters=10, int maxIterations=MAX_ITER, NumCDistType error=ERROR);
        Kmedians(NumC<NumCDataType>* data, int numOfClusters, int maxIterations=MAX_ITER, NumCDistType error=ERROR);

        ~Kmedians();

        void fit(NumC<NumCDataType>* trainData);
        void transform(ClusteringType clusteringType);
        void fit_transform(NumC<NumCDataType>* trainData, ClusteringType clusteringType);

        NumC<NumCDataType>* getCentroids();
        std::vector<Results*>  getResults();
        std::vector<NumCDistType> getSilhouettes();
        NumCDistType getObjectiveCost();
        
};

template class Kmedians<int>;
template class Kmedians<long>;
template class Kmedians<double>;

#endif