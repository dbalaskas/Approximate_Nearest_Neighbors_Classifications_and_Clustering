#ifndef LSH_H
#define LSH_H

#include "./numc.h"
#include "./lsh_hashtable.h"
#include "./prediction_results.h"

typedef vector<LSH_HashTable> HashTableList;

class LSH {
    private:
        NumC* data;
        int hashTableSize;
        int L;
        HashTableList hashTableList;
        int R;
    public:
        LSH();
        ~LSH();

        void fit(NumC* _data, int k, int _R, int _L, int _hashTableSize);
        void transform();
        void fit_transform(NumC* _data, int k, int _R, int _L, int _hashTableSize);
        Results predict_knn(Vector vector, int k);
        Results predict_rs(Vector vector, int r);
};

#endif