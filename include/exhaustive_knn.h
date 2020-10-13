#ifndef EXAUSTIVE_KNN_H
#define EXAUSTIVE_KNN_H
#include "./numc.h"
#include "./prediction_results.h"

template <typename NumCDataType> 
class ExaustiveKnn {
    private:
        NumC<NumCDataType>* data;
        int numOfNeighbors;

    public:
        ExaustiveKnn(int numOfNeighbors);
        ExaustiveKnn(NumC<NumCDataType>* data, int numOfNeighbors);
        ~ExaustiveKnn();

        void fit(NumC<NumCDataType>* trainData);
        Results* predict_knn(Vector<NumCDataType> vector);
        Results* predict_knn(NumC<NumCDataType>* testData);
};
#endif