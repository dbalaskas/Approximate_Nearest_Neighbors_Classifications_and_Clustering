#ifndef HC_H
#define HC_H

#include "./numc.h"
#include "./hashtable.h"
#include "./prediction_results.h"

class HyperCube {
    private:
        NumC* data;
        int hashTableSize;
        HashTable hashTable;
        int R;
    public:
        HyperCube(): data{NULL}, hashTableSize{0}, R{0} {};
        ~HyperCube();

        void fit(NumC* _data, int k, int _R, int _hashTableSize=0);
        void transform();
        void fit_transform(NumC* _data, int k, int _R, int _hashTableSize=0);
        Results predict_knn(Vector vector, int k, int maxPoints, int maxVertices);
        Results predict_rs(Vector vector, int r, int maxPoints, int maxVertices);
};

#endif