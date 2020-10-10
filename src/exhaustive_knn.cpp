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

ExaustiveKnn::ExaustiveKnn(int numOfNeighbors){
    this->numOfNeighbors = numOfNeighbors;
}

ExaustiveKnn::ExaustiveKnn(NumC* data, int numOfNeighbors){
    this->data = data;
    this->numOfNeighbors = numOfNeighbors;
}

ExaustiveKnn::~ExaustiveKnn(){
    this->numOfNeighbors = 0;
    this->data = NULL;
}

void ExaustiveKnn::fit(NumC* train_data){
    this->data = train_data;
}

// perform exaustivr search on every data for the near neighbours
Results ExaustiveKnn::predict_knn(Vector vector){

    // allocate results sruct for given k
    Results results(this->numOfNeighbors);
    ResultIndex resultIndex;

    // search every row data entry and find the k with minimun distance
    clock_t start = clock();
    for (int row = 0; row < this->data->getRows(); row++){
        // get the distance
        resultIndex.dist = NumC::dist(this->data->getVector(row), vector, 1);
        resultIndex.index = row;

        // add to results and the will figure out the best neighbors
        results.addResult(resultIndex);
    }
    clock_t end = clock();
    
    // set execution time
    results.setTime((double) (end - start) / CLOCKS_PER_SEC);

    return results;
}



///////////////// Test /////////////////
// int main(){

//     ExaustiveKnn knn(50);


//     NumC* inputData = PandaC::fromMNIST("../doc/input/train-images-idx3-ubyte");
//     NumC::print(inputData->getVector(0));

//     NumC* inputDatalabels = PandaC::fromMNISTlabels("../doc/input/train-labels-idx1-ubyte");
//     NumC::print(inputDatalabels->getVector(0));

//     knn.fit(inputData);


//     Results results;
//     results = knn.predict_knn(inputData->getVector(0));

//     results.print(inputDatalabels);


// }