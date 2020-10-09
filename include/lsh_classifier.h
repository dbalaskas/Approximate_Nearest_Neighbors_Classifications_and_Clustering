#ifndef LSH_H
#define LSH_H

#include "./numc.h"
#include "./lsh_hashtable.h"

class LSH {
    private:
        NumC* data;
        LSH_HashTable** hashTable;
        // int k;
        // int L;
        int R;
    public:
        LSH();
        LSH(int k, int L, int R);
        ~LSH();

        void fit(NumC* train_data);
        void transform();
        void fit_transform(NumC* train_data);
        int* predict_knn(Vector vector, double* time);
        int* predict_rs(Vector vector, double* time);
};

#endif