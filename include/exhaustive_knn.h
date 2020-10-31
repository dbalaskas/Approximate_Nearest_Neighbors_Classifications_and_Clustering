#ifndef EXHAUSTIVE_KNN_H
#define EXHAUSTIVE_KNN_H

#include "./numc.h"
#include "./prediction_results.h"

template <typename NumCDataType> 
class ExhaustiveKnn {
    private:
        NumC<NumCDataType>* data;   // train data
        int numOfNeighbors;         // Default number of neighbor

    public:
        ExhaustiveKnn(int numOfNeighbors);
        ExhaustiveKnn(NumC<NumCDataType>* data, int numOfNeighbors);
        ~ExhaustiveKnn();

        // Gets train data.
        void fit(NumC<NumCDataType>* trainData);

        // Computes the k-NN of the given vector.
        Results* predict_knn(Vector<NumCDataType> vector, int numOfNeighbors_=0);
        // Computes the k-NN of the given matrix. Calls the previous method for each row.
        Results* predict_knn(NumC<NumCDataType>* testData, int numOfNeighbors_=0);
};

// Define the templates of Hypercube
template class ExhaustiveKnn<int>;
template class ExhaustiveKnn<long>;
template class ExhaustiveKnn<double>;

#endif