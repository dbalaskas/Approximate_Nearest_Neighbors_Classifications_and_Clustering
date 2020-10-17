#ifndef LSH_H
#define LSH_H

#include "./numc.h"
#include "./hashtable.h"
#include "./prediction_results.h"

// typedef vector<HashTable<NumCDataType>> HashTableList;


template <typename NumCDataType>
class LSHashing {

    private:
        int N;
        int L;
        int k;
        int w;
        NumC<NumCDataType>* data;
        HashTable<NumCDataType>** hashTableList;

    public:
        LSHashing():data{NULL}, hashTableList{NULL}, N{1}, L{5}, k{4}, w{10} {};
        LSHashing(int N, int L = 5, int k = 4, int w = 10);
        ~LSHashing();

        void fit(NumC<NumCDataType>* _data);
        void transform();
        void fit_transform(NumC<NumCDataType>* _data);

        Results* predict_knn(Vector<NumCDataType> vector, int N=0);
        Results* predict_knn(NumC<NumCDataType>* testData, int N=0);

        Results* predict_rs(Vector<NumCDataType> vector, double r);
};

#endif