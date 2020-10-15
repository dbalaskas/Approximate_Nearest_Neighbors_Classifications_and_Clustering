#ifndef LSH_H
#define LSH_H

#include "./numc.h"
#include "./hashtable.h"
#include "./prediction_results.h"

// typedef vector<HashTable<NumCDataType>> HashTableList;


template <typename NumCDataType>
class LSH {

    private:
        int N;
        int L;
        int k;
        int w;
        NumC<NumCDataType>* data;
        std::vector<HashTable<NumCDataType>> hashTableList;

    public:
        LSH():data{NULL}, N{1}, L{5}, k{4}, w{10} {};
        LSH(int N, int L = 5, int k = 4, int w = 10);
        ~LSH();

        void fit(NumC<NumCDataType>* _data);
        void transform();
        void fit_transform(NumC<NumCDataType>* _data);

        Results* predict_knn(Vector<NumCDataType> vector, int k);
        Results* predict_knn(NumC<NumCDataType>* testData, int k);

        Results* predict_rs(Vector<NumCDataType> vector, int r);
        Results* predict_rs(NumC<NumCDataType>* testData, int r);
};

#endif