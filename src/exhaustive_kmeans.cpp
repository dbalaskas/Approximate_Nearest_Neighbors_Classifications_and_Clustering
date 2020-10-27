#include "../include/exhaustive_kmeans.h"
#include "../include/pandac.h"

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename NumCDataType> 
ExhaustiveKmeans<NumCDataType>::ExhaustiveKmeans(int numOfClusters){
    this->numOfClusters = numOfClusters;
    this->numOfDimensions = 0;
    this->overallSilhouette = 0.0;
    this->centroids = NULL;
    this->objectiveCost = 0.0;
}

template <typename NumCDataType> 
ExhaustiveKmeans<NumCDataType>::ExhaustiveKmeans(NumC<NumCDataType>* data, int numOfClusters){
    this->data = data;
    this->numOfClusters = numOfClusters;
    this->numOfDimensions = this->data->getCols();
    this->numOfPoints = this->data->getRows();
    this->silhouette.reserve(numOfClusters);
    this->overallSilhouette = 0.0;
    this->centroids = NULL;
    this->objectiveCost = 0.0;
}

template <typename NumCDataType> 
ExhaustiveKmeans<NumCDataType>::~ExhaustiveKmeans(){
    this->numOfClusters = 0;

    if (this->data != NULL) {
        this->data = NULL;
    }
    if (this->centroids != NULL) {
        delete this->centroids;
        this->centroids = NULL;
    }


}

template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::randomInit(){
    // allocate random for centroids
    if (this->centroids != NULL) {
        delete this->centroids;
        this->centroids = NULL;
    }
    this->centroids = new NumC<NumCDataType>(this->numOfClusters, this->numOfDimensions);
    NumCDataType max = this->data->max();
    this->centroids->random(max);
}

template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::kmeansInit(){

    srand(time(NULL));
    std::random_device randomDevice; 
    std::mt19937 generator(randomDevice()); 


    // allocate space for the first centroid
    if (this->centroids != NULL) {
        delete this->centroids;
        this->centroids = NULL;
    }
    this->centroids = new NumC<NumCDataType>(1, this->numOfDimensions);

    ExhaustiveKnn<NumCDataType>* initEstimator = new ExhaustiveKnn<NumCDataType>(1);
    Results* results;
    NumCDistType lastElement;
    NumCDistType randomX;
    NumCIndexType randomXindex;
    // pick a random cantroid to start with
    randomXindex = rand()%this->numOfPoints;
    cout << "FIND--> " <<randomXindex <<endl;
    this->centroids->addVector(this->data->getVector(randomXindex), 0);
    // this->centroids->print();

    // start kmeans++
    for (int centroidIndex = 1; centroidIndex < this->numOfClusters; centroidIndex++){
        
        // get the min distance of each point to centroids
        initEstimator->fit(this->centroids);
        results = initEstimator->predict_knn(this->data);

        // normalize and accumulate the values
        // square
        results->resultsDistArray.square();
        results->resultsDistArray.normalize();
        results->resultsDistArray.cumulative();

        cout <<endl<<"LAST-> " << results->resultsDistArray.getLast()<<endl;

        // get a random number between [0,P(n−t)]
        lastElement = results->resultsDistArray.getLast();
        std::uniform_real_distribution<NumCDistType> distribution(0,lastElement);
        randomX = (NumCDistType)distribution(generator);
        
        // get the index between
        randomXindex = results->resultsDistArray.find(randomX);

        // add to centroids this element of this index
        this->centroids->appendVector(this->data->getVector(randomXindex));
        
        cout <<"ranosm-> " << randomX<<endl;
        cout << "FIND--> " <<randomXindex <<endl;
        // results->resultsDistArray.print();
        delete results;
    }
    
    delete initEstimator;
}


template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::fit(NumC<NumCDataType>* trainData){
    this->data = trainData;
    this->numOfDimensions = this->data->getCols();
    this->numOfPoints = this->data->getRows();

    // init centroids
    // this->randomInit();
    this->kmeansInit();

}

template <typename NumCDataType> 
NumC<NumCDataType>* ExhaustiveKmeans<NumCDataType>::getCentroids(){
    return this->centroids;
}

// calculate and get the indexes of images that belong to known centroids
template <typename NumCDataType> 
vector<Results*> ExhaustiveKmeans<NumCDataType>::getResults(){

    // allocate space for new results
    vector<Results*> totalResults(this->numOfClusters); 
    ResultsComparator* resultsComparator;

    ExhaustiveKnn<NumCDataType>* knnEstimator = new ExhaustiveKnn<NumCDataType>(1);
    knnEstimator->fit(this->centroids);
    Results* resultsKnn = knnEstimator->predict_knn(this->data);

    for (int centroidIndex = 0; centroidIndex < this->numOfClusters; centroidIndex++){

        resultsComparator = new ResultsComparator(0);

        for (int resultsIndex = 0; resultsIndex < resultsKnn->resultsIndexArray.getRows(); resultsIndex++){
            if ( resultsKnn->resultsIndexArray.getElement(resultsIndex, 0) ==  centroidIndex){
                resultsComparator->addResult(resultsIndex, resultsKnn->resultsDistArray.getElement(resultsIndex, 0));
            }
        }
        totalResults[centroidIndex] = resultsComparator->getResults();
        delete resultsComparator;
    }

    delete resultsKnn;
    delete knnEstimator;
    return totalResults;
}

template <typename NumCDataType> 
NumCDistType ExhaustiveKmeans<NumCDataType>::calculateSilhouette(NumCDistType distA, NumCDistType distB){

    if (distA < distB){
        return 1.0 - (distA / distB);
    }
    else if (distA == distB){
        return 0;
    }
    else if (distA > distB){
        return  (distB / distA) - 1.0;
    }

}

template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::getSilhouette(Results* results){
    std::vector<NumCDistType> meanDistA(this->numOfPoints / this->numOfClusters);
    std::vector<NumCDistType> meanDistB(this->numOfPoints / this->numOfClusters);
    NumCDistType distA = 0;
    NumCDistType distB = 0;
    NumCDistType meanA = 0;
    NumCDistType meanB = 0;

    this->silhouette.clear();

    for (int centroidIndex = 0; centroidIndex < this->numOfClusters; centroidIndex++){

        meanDistA.clear();
        meanDistB.clear();
        for (int resultsIndex = 0; resultsIndex < results->resultsIndexArray.getRows(); resultsIndex++){

            if ( results->resultsIndexArray.getElement(resultsIndex, 0) ==  centroidIndex){
                distA = results->resultsDistArray.getElement(resultsIndex, 0);
                distB = results->resultsDistArray.getElement(resultsIndex, 1);
                meanDistA.push_back(distA);
                meanDistB.push_back(distB);
            }
        }
        // get the silhouette
        meanA = std::accumulate(meanDistA.begin(), meanDistA.end(), 0.0) / (NumCDistType)meanDistA.size();
        meanB = std::accumulate(meanDistB.begin(), meanDistB.end(), 0.0) / (NumCDistType)meanDistB.size();
        this->silhouette.push_back(calculateSilhouette( meanA, meanB));
    }
    overallSilhouette =  std::accumulate(this->silhouette.begin(), this->silhouette.end(), 0.0) / (NumCDistType)(this->numOfClusters);
    for (int i = 0; i < 10; i++){
            cout << this->silhouette[i] << ", ";
    }
    cout << "\n[" << this->overallSilhouette << "]"<<endl;
}

template <typename NumCDataType> 
NumCDistType ExhaustiveKmeans<NumCDataType>::getObjectiveCost(Results* results){
    NumCDistType sum;
    for (int i = 0; i < results->resultsDistArray.getRows(); i++){
        sum += results->resultsDistArray.getElement(i, 0);
    }
    return sum;
}

template <typename NumCDataType> 
NumCDistType ExhaustiveKmeans<NumCDataType>::getObjectiveCost(){
    return this->objectiveCost;
}

template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::medianCentroidsUpdate(Results* results){

    std::vector<NumCIndexType> medianVector(this->numOfPoints / this->numOfClusters);
    NumCDataType medianElement = 0;
    NumCDataType median = 0;
    int size = 0;

    for (int centroidIndex = 0; centroidIndex < this->numOfClusters; centroidIndex++){
        for (int dimension = 0; dimension < this->numOfDimensions; dimension++){  
            medianVector.clear();
            for (int resultsIndex = 0; resultsIndex < results->resultsIndexArray.getRows(); resultsIndex++){
                if ( results->resultsIndexArray.getElement(resultsIndex, 0) ==  centroidIndex){
                    medianElement = this->data->getElement(resultsIndex, dimension);
                    medianVector.push_back(medianElement);
                }
            }
            // calculate median for that dimension
            size = medianVector.size();
            if (size != 0){
                std::sort(medianVector.begin(), medianVector.end());
                if (size % 2 == 0){
                    median = (medianVector[size/2 - 1] + medianVector[size/2]) / 2;}
                else {
                    median = medianVector[size/2];}
            }
            else{
                median = 0;
            }
            centroids->addElement( median, centroidIndex, dimension);      
        }
    }
}

template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::transform(ClusteringType clusteringType){

    if (clusteringType == LLOYDS_CLUSTERING) {
        transform_LLOYDS_CLUSTERING();
    } else if (clusteringType == LSH_CLUSTERING) {
        // transform_LSH_CLUSTERING();
    } else if (clusteringType == HC_CLUSTERING) {
        // transform_HC_CLUSTERING();
    }

}

template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::transform_LLOYDS_CLUSTERING(){

    NumCDistType prev_overallSilhouette;
    NumCDistType silhouette_error;
    Results* results;

    ExhaustiveKnn<NumCDataType>* knnEstimator = new ExhaustiveKnn<NumCDataType>(2);
    knnEstimator->fit(this->centroids);


    for (int i = 0; i < 100; i++){

        clock_t start = clock();
        results = knnEstimator->predict_knn(this->data);
        clock_t end_knn = clock();
        cout <<endl<<"KNN TIME [" << ((double) (end_knn - start) / CLOCKS_PER_SEC) <<"]"<<endl; 

        // find the median for each centroid
        medianCentroidsUpdate(results);
        clock_t end_median = clock();
        cout <<"MEDIAN TIME [" << ((double) (end_median - end_knn) / CLOCKS_PER_SEC) <<"]"<<endl;

        prev_overallSilhouette = this->overallSilhouette;
        getSilhouette(results);

        this->objectiveCost = getObjectiveCost(results);
        cout << "COST: [" << this->objectiveCost << "]" <<endl;

        delete results;
        
        // if prev = new then optimization has converged
        // by 0.1%
        silhouette_error = abs(this->overallSilhouette - prev_overallSilhouette) / this->overallSilhouette;
        if (silhouette_error < 0.001){
            cout << "Kmeans Converged in [" << i<< "] iterations"<<endl;
            break;
        }
    }
    delete knnEstimator;
}




int main(){

    ExhaustiveKmeans<int> kmeans(10);


    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte", 60000);
    // NumC<int>::print(inputData->getVector(0));
    // NumC<int>::printSparse(inputData->getVector(1));


    NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte", 60000);
//     // NumC<int>::print(inputDatalabels->getVector(0));

    kmeans.fit(inputData);

    NumC<int>* inputData_ = new NumC<int>(10, inputData->getCols(), true);
    for (int i = 0; i < 10; i++){
        inputData_->addVector(inputData->getVector(i), i);
    }

    kmeans.transform(LLOYDS_CLUSTERING);

    std::vector<Results*> res;
    res = kmeans.getResults();
    for (int i = 0; i < res.size(); i++){
        // ResultsComparator::print(res[i], inputDatalabels);
        delete res[i];
    }
    

    // Results* results;
    
    // ResultsComparator::print(results, inputDatalabels);
    // delete results;

    delete inputData_;
    delete inputData;
    delete inputDatalabels;

}
