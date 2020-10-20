#include "../include/exhaustive_knn.h"

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

template <typename NumCDataType> 
ExhaustiveKnn<NumCDataType>::ExhaustiveKnn(int numOfNeighbors){
    this->numOfNeighbors = numOfNeighbors;
}

template <typename NumCDataType> 
ExhaustiveKnn<NumCDataType>::ExhaustiveKnn(NumC<NumCDataType>* data, int numOfNeighbors){
    this->data = data;
    this->numOfNeighbors = numOfNeighbors;
}

template <typename NumCDataType> 
ExhaustiveKnn<NumCDataType>::~ExhaustiveKnn(){
    this->numOfNeighbors = 0;
    this->data = NULL;
}

template <typename NumCDataType> 
void ExhaustiveKnn<NumCDataType>::fit(NumC<NumCDataType>* trainData){
    this->data = trainData;
}

// perform exaustivr search on every data for the near neighbours
template <typename NumCDataType> 
Results* ExhaustiveKnn<NumCDataType>::predict_knn(Vector<NumCDataType> vector){

    // comparator to get best results distances
    ResultsComparator resultsComparator(this->numOfNeighbors);

    // search every row data entry and find the k with minimun distance
    clock_t start = clock();
    for (int row = 0; row < this->data->getRows(); row++){

        // add to results and the will figure out the best neighbors
        resultsComparator.addResult(row, NumC<NumCDataType>::distSparse(this->data->getVector(row), vector, 1));

    }
    clock_t end = clock();
    
    Results* results = resultsComparator.getResults();
    // results 
    results->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);

    return results;
}

// perform Exhaustive search on every data for the near neighbours for every vector in query
template <typename NumCDataType> 
Results* ExhaustiveKnn<NumCDataType>::predict_knn(NumC<NumCDataType>* testData){

    int numOfQueries = testData->getRows();
    // allocate results sruct for given k
    Results* totalResults = new Results(numOfQueries, this->numOfNeighbors); 
    Results* queryResults;

    // search every row data entry and find the k with minimun distance
    clock_t start = clock();
    for (int query = 0; query < numOfQueries; query++){

        // add to results the results of every query
        queryResults = this->predict_knn(testData->getVector(query));
        totalResults->resultsIndexArray.addVector(queryResults->resultsIndexArray.getVector(0), query);
        totalResults->resultsDistArray.addVector(queryResults->resultsDistArray.getVector(0), query);

        // free query results
        delete queryResults;

    }
    clock_t end = clock();
    
    // results 
    totalResults->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);

    return totalResults;
}

NumC<double> pipi(int input){
    NumC<double> nn(6,input);
    for (int i = 0; i < nn.getRows(); i++){
        for (int j = 0; j < nn.getCols(); j++){
            nn.addElement(i, i, j);
        }
        
    }
    nn.print();
    return nn;
}

// #include "../include/pandac.h"
// int main(){

//     ExhaustiveKnn<int> knn(10);


//     NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
//     // NumC<int>::print(inputData->getVector(0));
//     // NumC<int>::printSparse(inputData->getVector(1));


//     NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");
//     // NumC<int>::print(inputDatalabels->getVector(0));

//     knn.fit(inputData);

//     NumC<int>* inputData_ = new NumC<int>(10, inputData->getCols(), true);
//     for (int i = 0; i < 10; i++){
//         inputData_->addVector(inputData->getVector(i), i);
//     }

//     // HashFunction<int> hash(5, 10, 10);
    
//     // cout << NumC<int>::dist(inputData->getVector(0), inputData->getVector(1), 1)<<endl;

//     Results* results;
//     results = knn.predict_knn(inputData_);
//     // results = knn.predict_knn(inputData->getVector(6));
    
//     // results.print();
//     results->resultsIndexArray.print();
//     ResultsComparator::print(results, inputDatalabels);

//     delete results;


//     // clock_t start = clock();
//     // for (long query = 0; query < 60000 * 10; query++){
//     //     NumC<int>::distSparse(inputData->getVector(0), inputData->getVector(1), 1);
//     // }
//     // clock_t end = clock();
    
//     // // results 
//     // cout << endl<<"TIMEEEEEEE "<<((double) (end - start) / CLOCKS_PER_SEC) <<endl;


    
//     // NumC<double> matrix(3,5);
//     // matrix = NumC<double>(5,10);
//     // matrix.randn(100);
//     // matrix.print();
//     // matrix = pipi(10);

//     // matrix.addVector(matrix.getVector(0), 3);
//     // matrix.print();
//     // matrix.transpose();
//     // matrix.print();

//     delete inputData_;
//     delete inputData;
//     delete inputDatalabels;
    

// }