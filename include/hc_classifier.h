#ifndef HC_H
#define HC_H

#include "./numc.h"
#include "./hc_hashtable.h"

class HyperCube {
    private:
        NumC* data;
        HC_HashTable** hashTable;
        int R;
    public:
        HyperCube(int R);
        ~HyperCube();

        void fit(NumC* train_data);
        void transform();
        void fit_transform(NumC* train_data);
        int* predict_knn(Vector vector, double* time);
        int* predict_rs(Vector vector, double* time);
};

#endif