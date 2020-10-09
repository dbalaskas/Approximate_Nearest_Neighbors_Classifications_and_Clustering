#include "../include/exhaustive_knn.h"
#include "../include/prediction_results.h"
#include "../include/numc.h"

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>

ExaustiveKnn::ExaustiveKnn(int k){
    this->k = k;
}

ExaustiveKnn::ExaustiveKnn(NumC* data, int k){
    this->data = data;
    this->k = k;
}

ExaustiveKnn::~ExaustiveKnn(){
    this->k = 0;
    this->data = NULL;
}

void ExaustiveKnn::fit(NumC* train_data){
    this->data = train_data;
}

// perform exaustivr search on every data for the near neighbours
Results ExaustiveKnn::predict_knn(Vector vector){

    // allocate results sruct for given k
    Results results(this->data->getRows());
    PointIndex pointIndex;
    std::priority_queue<double> pQ;
    // results.resultList.reserve(this->data->getRows());

    // search every row data entry and find the k with minimun distance
    for (int row = 0; row < this->data->getRows(); row++){
        std::cout << "naii\n";
        // get the distance
        pointIndex.dist = NumC::dist(this->data->getVector(row), vector, 1);
        pointIndex.intex = row;
        results.resultList.push_back(pointIndex);
    }
    

    return results;
}

// int main(){

//     ExaustiveKnn knn(5);

//     NumC nn(6,33);
//     for (int i = 0; i < nn.getRows(); i++){
//         for (int j = 0; j < nn.getCols(); j++){
//             nn.addElement(i, i, j);
//         }
        
//     }

//     knn.fit(&nn);


//     Results results;
//     results = knn.predict_knn(nn.getVector(2));
//     for (int row = 0; row < nn.getRows(); row++){
//         std::cout << results.resultList.at(row).dist << "\n";
//     }

// }