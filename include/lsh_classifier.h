#ifndef LSH_H
#define LSH_H

#include <vector>
#include "./numc.h"
#include "./hashtable.h"
#include "./prediction_results.h"

// typedef vector<HashTable<NumCDataType>> HashTableList;


template <typename NumCDataType>
class LSHashing {

    private:
        int L;
        int k;
        int w;
        NumC<NumCDataType>* data;
        HashTable<NumCDataType>** hashTableList;

    public:
        LSHashing(): L{5}, k{4}, w{10}, data{NULL}, hashTableList{NULL} {};
        LSHashing(int L = 5, int k = 4, int w = 50000);
        ~LSHashing();

        void fit(NumC<NumCDataType>* _data);
        void transform();
        void fit_transform(NumC<NumCDataType>* _data);

        Results* predict_knn(Vector<NumCDataType> vector, int N);
        Results* predict_knn(NumC<NumCDataType>* testData, int N);

        Results* predict_rs(Vector<NumCDataType> vector, double r);
        std::vector<Results*> predict_rs(NumC<NumCDataType>* testData, int r);
};

template class LSHashing<int>;
template class LSHashing<long>;
template class LSHashing<double>;

#endif