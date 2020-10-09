#ifndef HC_H
#define HC_H

#include "./numc.h"
#include "./hc_hashtable.h"
#include "./prediction_results.h"

class HyperCube {
    private:
        NumC* data;
        int hashTableSize;
        HC_HashTable hashTable;
        int R;
    public:
        HyperCube();
        ~HyperCube();

        void fit(NumC* _data, int k, int _R, int _hashTableSize);
        void transform();
        void fit_transform(NumC* _data, int k, int _R, int _hashTableSize);
        Results predict_knn(Vector vector, int k, int maxPoints, int maxVertices);
        Results predict_rs(Vector vector, int r, int maxPoints, int maxVertices);
};

#endif