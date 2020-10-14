#ifndef LSH_H
#define LSH_H

#include "./numc.h"
#include "./lsh_hashtable.h"
#include "./prediction_results.h"

typedef vector<LSH_HashTable> HashTableList;


template <typename NumCDataType>
class LSH {

    private:
        NumC<NumCDataType>* data;
        int hashTableSize;
        int L;
        HashTableList hashTableList;
        int R;

    public:
        LSH();
        ~LSH();

        void fit(NumC<NumCDataType>* _data, int k, int _R, int _L, int _hashTableSize=0);
        void transform();
        void fit_transform(NumC<NumCDataType>* _data, int k, int _R, int _L, int _hashTableSize=0);

        Results* predict_knn(Vector<NumCDataType> vector, int k);
        Results* predict_knn(NumC<NumCDataType>* testData, int k);

        Results* predict_rs(Vector<NumCDataType> vector, int r);
        Results* predict_rs(NumC<NumCDataType>* testData, int r);
};

#endif