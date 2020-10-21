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
        this->knnEstimator = NULL;
    }
    if (this->centroidsMap != NULL) {
        delete this->centroidsMap;
        this->centroidsMap = NULL;
    }

}

template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::randomInit(){
    // allocate random for centroids
    this->centroids = new NumC<NumCDataType>(this->numOfClusters, this->numOfDimensions);
    NumCDataType max = this->data->max();
    this->centroids->random(max);
}

template <typename NumCDataType> 
void ExhaustiveKmeans<NumCDataType>::kmeansInit(){

    srand(time(NULL));
    std::random_device randomDevice; 
    std::mt19937 generator(randomDevice()); 

    // NumC<double> test(2,10);
    // test.random(10);
    // // for (int i = 0; i < 10; i++){
    // //     test.addElement(0.0+(rand()%100),0,i );
    // // }
    
    // test.print();
    // cout << test.max()<<endl;

    // test.square();
    // test.print();


    // test.normalize();
    // test.print();

    // test.cumulative();
    // test.print();

    // allocate space for the first centroid
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
    this->randomInit();
    // this->kmeansInit();

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

    // std::priority_queue<NumCIndexType, std::vector<NumCIndexType>, std::greater<NumCIndexType> > great;
    // std::priority_queue<NumCIndexType, std::vector<NumCIndexType>, std::less<NumCIndexType> > small;
    // great.push(std::numeric_limits<NumCIndexType>::max());
    // small.push(std::numeric_limits<NumCIndexType>::min());

    NumCDataType medianElement = 0;
    NumCDataType median = 0;
    NumCDistType prev_overallSilhouette;
    NumCDistType silhouette_error;
    int size = 0;
    Results* results;
    this->knnEstimator->fit(this->centroids);

    for (int i = 0; i < 100; i++){
        clock_t start = clock();
        results = this->knnEstimator->predict_knn(this->data);
        clock_t end_knn = clock();
        cout <<endl<<"KNN TIME [" << ((double) (end_knn - start) / CLOCKS_PER_SEC) <<"]"<<endl; 
        // find the median for each centroid
        for (int centroidIndex = 0; centroidIndex < this->numOfClusters; centroidIndex++){

            for (int dimension = 0; dimension < this->numOfDimensions; dimension++){
                
                medianVector.clear();
                // // clear priority queues
                // great = std::priority_queue<NumCIndexType, std::vector<NumCIndexType>, std::greater<NumCIndexType> >();
                // small = std::priority_queue<NumCIndexType, std::vector<NumCIndexType>, std::less<NumCIndexType> >();
                // great.push(std::numeric_limits<NumCIndexType>::max());
                // small.push(std::numeric_limits<NumCIndexType>::min());

                for (int resultsIndex = 0; resultsIndex < results->resultsIndexArray.getRows(); resultsIndex++){
                    if ( results->resultsIndexArray.getElement(resultsIndex, 0) ==  centroidIndex){
                        medianElement = this->data->getElement(resultsIndex, dimension);

                        medianVector.push_back(medianElement);

                        // // push to priotity queues
                        // if(medianElement >= great.top())
                        //     great.push(medianElement);
                        // else 
                        //     small.push(medianElement);

                        // // rebalance priority queues
                        // if( (great.size() - small.size()) == 2){ 
                        //     small.push(great.top());   
                        //     great.pop();
                        // }
                        //  else if( (small.size() - great.size()) == 2){ 
                        //     great.push(small.top()); 
                        //     small.pop(); 
                        // }

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

                // // get the median in logn time
                // if(great.size() == small.size())
                //     median  = (great.top() + small.top()) / 2;
                // else if(great.size() > small.size()) 
                //     median = great.top();
                // else 
                //     median = small.top();

                centroids->addElement( median, centroidIndex, dimension);      
            }
        }
        clock_t end_median = clock();
        cout <<endl<<"MEDIAN TIME [" << ((double) (end_median - end_knn) / CLOCKS_PER_SEC) <<"]"<<endl;

        prev_overallSilhouette = this->overallSilhouette;
        getSilhouette(results);

        for (int i = 0; i < 10; i++){
            cout << this->silhouette[i] << ", ";
        }
        cout << "\n[" << this->overallSilhouette << "]"<<endl;

        delete results;
        
        // if prev = new then optimization has converged
        // by 0.1%
        silhouette_error = abs(this->overallSilhouette - prev_overallSilhouette) / this->overallSilhouette;
        if (silhouette_error < 0.001){
            cout << "Kmeans Converged in [" << i<< "] iterations"<<endl;
            break;
        }
    
    }

}



int main(){

    ExhaustiveKmeans<int> kmeans(10);


    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte", 60000);
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
