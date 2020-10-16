#ifndef HC_H
#define HC_H

#include <vector>
#include "./numc.h"
#include "./hashtable.h"
#include "./prediction_results.h"

template <typename NumCDataType>
class HyperCube {
    private:
        NumC<NumCDataType>* data;
        int hashTableSize;
        HashTable<NumCDataType>* hashTable;
        int k;
    public:
        HyperCube(): hashTable{NULL}, data{NULL}, hashTableSize{0}, k{0} {};
        ~HyperCube();

        void fit(NumC<NumCDataType>* _data);
        void transform();
        void fit_transform(NumC<NumCDataType>* _data);
        Results* predict_knn(Vector<NumCDataType> vector, int k, int maxPoints, int maxVertices);
        Results* predict_knn(NumC<NumCDataType>* testData);
        std::vector<int> predict_rs(Vector<NumCDataType> vector, int r, int maxPoints, int maxVertices);
};

template class HyperCube<int>;
template class HyperCube<long>;
template class HyperCube<double>;

#endif