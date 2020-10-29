#ifndef EXHAUSTIVE_KNN_H
#define EXHAUSTIVE_KNN_H

#include "./numc.h"
#include "./prediction_results.h"

template <typename NumCDataType> 
class ExhaustiveKnn {
    private:
        NumC<NumCDataType>* data;
        int numOfNeighbors;

    public:
        ExhaustiveKnn(int numOfNeighbors);
        ExhaustiveKnn(NumC<NumCDataType>* data, int numOfNeighbors);
        ~ExhaustiveKnn();

        void fit(NumC<NumCDataType>* trainData);
        Results* predict_knn(Vector<NumCDataType> vector, int numOfNeighbors_=0);
        Results* predict_knn(NumC<NumCDataType>* testData, int numOfNeighbors_=0);
};

template class ExhaustiveKnn<int>;
template class ExhaustiveKnn<long>;
template class ExhaustiveKnn<double>;

#endif