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
}

template <typename NumCDataType> 
ExhaustiveKmeans<NumCDataType>::ExhaustiveKmeans(NumC<NumCDataType>* data, int numOfClusters){
    this->data = data;
    this->numOfClusters = numOfClusters;
    this->numOfDimensions = this->data->getCols();
    this->numOfPoints = this->data->getRows();
    this->silhouette.reserve(numOfClusters);
    this->overallSilhouette = 0.0;
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
    if (this->knnEstimator != NULL) {
        delete this->knnEstimator;
        this->centroids = NULL;
    }
    if (this->centroidsMap != NULL) {
        delete this->centroidsMap;
        this->centroids = NULL;
    }

}

template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::fit(NumC<NumCDataType>* trainData){
    this->data = trainData;
    this->numOfDimensions = this->data->getCols();
    this->numOfPoints = this->data->getRows();

    // allocate random for centroids
    this->centroids = new NumC<NumCDataType>(this->numOfClusters, this->numOfDimensions);
    this->centroids->random(256);

    // allocate array for the centroids that belong to each point
    this->centroidsMap = new NumC<NumCDataType>(this->numOfPoints, 1);

    // allocate memory for the knn estimaor
    this->knnEstimator = new ExhaustiveKnn<NumCDataType>(2);
    this->knnEstimator->fit(this->centroids);
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

}

template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::transform(){
    // NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");
    std::vector<NumCIndexType> medianVector(this->numOfPoints / this->numOfClusters);

    NumCDataType medianElement = 0;
    NumCDataType median = 0;
    int size = 0;
    Results* results;
    this->knnEstimator->fit(this->centroids);

    for (int i = 0; i < 100; i++){

        results = this->knnEstimator->predict_knn(this->data);
        
        // find the median for each centroid
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
                        median = (medianVector[size/2 - 1] + medianVector[size/2]) / 2;
                    }
                    else {
                        median = medianVector[size/2];
                    }
                }
                else{
                    median = 0;
                }
                centroids->addElement( median, centroidIndex, dimension);      
            }
        }

        getSilhouette(results);

        for (int i = 0; i < 10; i++){
            cout << this->silhouette[i] << ", ";
        }
        cout << "\n[" << this->overallSilhouette << "]"<<endl;
        
        delete results;
    
    }

}



int main(){

    ExhaustiveKmeans<int> kmeans(10);


    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
    // NumC<int>::print(inputData->getVector(0));
    // NumC<int>::printSparse(inputData->getVector(1));


    // NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");
//     // NumC<int>::print(inputDatalabels->getVector(0));

    kmeans.fit(inputData);

    NumC<int>* inputData_ = new NumC<int>(10, inputData->getCols(), true);
    for (int i = 0; i < 10; i++){
        inputData_->addVector(inputData->getVector(i), i);
    }

    kmeans.transform();
    

    // Results* results;
    
    // ResultsComparator::print(results, inputDatalabels);
    // delete results;

    delete inputData_;
    delete inputData;
    // delete inputDatalabels;

}
