#include "../include/exhaustive_knn.h"
#include "../include/prediction_results.h"
#include "../include/numc.h"
#include "../include/pandac.h"

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

template <typename NumCDataType> 
ExaustiveKnn<NumCDataType>::ExaustiveKnn(int numOfNeighbors){
    this->numOfNeighbors = numOfNeighbors;
}

template <typename NumCDataType> 
ExaustiveKnn<NumCDataType>::ExaustiveKnn(NumC<NumCDataType>* data, int numOfNeighbors){
    this->data = data;
    this->numOfNeighbors = numOfNeighbors;
}

template <typename NumCDataType> 
ExaustiveKnn<NumCDataType>::~ExaustiveKnn(){
    this->numOfNeighbors = 0;
    this->data = NULL;
}

template <typename NumCDataType> 
void ExaustiveKnn<NumCDataType>::fit(NumC<NumCDataType>* train_data){
    this->data = train_data;
}

// perform exaustivr search on every data for the near neighbours
template <typename NumCDataType> 
Results ExaustiveKnn<NumCDataType>::predict_knn(Vector<NumCDataType> vector){

    // allocate results sruct for given k
    Results results(this->numOfNeighbors);
    ResultIndex resultIndex;

    // search every row data entry and find the k with minimun distance
    clock_t start = clock();
    for (int row = 0; row < this->data->getRows(); row++){
        // get the distance
        resultIndex.dist = NumC<NumCDataType>::dist(this->data->getVector(row), vector, 1);
        resultIndex.index = row;

        // add to results and the will figure out the best neighbors
        results.addResult(resultIndex);
    }
    clock_t end = clock();
    
    // set execution time
    results.setTime((double) (end - start) / CLOCKS_PER_SEC);

    return results;
}

// NumC<double> pipi(int input){
//     NumC<double> nn(6,input);
//     for (int i = 0; i < nn.getRows(); i++){
//         for (int j = 0; j < nn.getCols(); j++){
//             nn.addElement(i, i, j);
//         }
        
//     }
//     nn.print();
//     return nn;
// }


// ///////////////// Test /////////////////
// int main(){

//     ExaustiveKnn<int> knn(50);


//     NumC<int>* inputData = PandaC::fromMNIST("../doc/input/train-images-idx3-ubyte");
//     NumC<int>::print(inputData->getVector(0));

//     NumC<int>* inputDatalabels = PandaC::fromMNISTlabels("../doc/input/train-labels-idx1-ubyte");
//     NumC<int>::print(inputDatalabels->getVector(0));

//     knn.fit(inputData);


//     Results results;
//     results = knn.predict_knn(inputData->getVector(0));

//     results.print(inputDatalabels);

//     NumC<double> matrix = pipi(3);
//     matrix.print();
//     matrix = pipi(10);

//     matrix.addVector(matrix.getVector(0), 3);
//     matrix.print();
//     matrix.transpose();
//     matrix.print();


// }