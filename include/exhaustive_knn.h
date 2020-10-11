#ifndef EXAUSTIVE_KNN_H
#define EXAUSTIVE_KNN_H
#include "./numc.h"
#include "./prediction_results.h"

class ExaustiveKnn {
    private:
        NumC* data;
        int numOfNeighbors;

    public:
        ExaustiveKnn(int numOfNeighbors);
        ExaustiveKnn(NumC* data, int numOfNeighbors);
        ~ExaustiveKnn();

        void fit(NumC* train_data);
        Results predict_knn(Vector vector);
};
#endif