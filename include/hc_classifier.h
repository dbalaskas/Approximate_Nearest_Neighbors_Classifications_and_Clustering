#ifndef HC_H
#define HC_H

#include "./numc.h"
#include "./hashtable.h"
#include "./prediction_results.h"

template <typename NumCDataType>
class HyperCube {
    private:
        NumC<NumCDataType>* data;
        int hashTableSize;
        HashTable<NumCDataType> hashTable;
        int k;
    public:
        HyperCube(): hashTable(HC), data{NULL}, hashTableSize{0}, k{0} {};
        ~HyperCube();

        void fit(NumC<NumCDataType>* _data);
        void transform();
        void fit_transform(NumC<NumCDataType>* _data);
        // Results predict_knn(Vector<NumCDataType> vector, int k, int maxPoints, int maxVertices);
        // Results predict_rs(Vector<NumCDataType> vector, int r, int maxPoints, int maxVertices);
};

#endif