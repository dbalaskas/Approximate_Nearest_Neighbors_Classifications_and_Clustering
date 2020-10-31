#ifndef HC_H
#define HC_H

#include <vector>
#include "./numc.h"
#include "./hashtable.h"
#include "./prediction_results.h"
#include "./exhaustive_knn.h"

template <typename NumCDataType>
class HyperCube {
    private:
        int w;
        int d;
        NumC<NumCDataType>* data;
        int hashTableSize;
        HashTable<NumCDataType>* hashTable;

        void get_nearestHashes(unsigned int vector, int k, int changesLeft, std::vector<unsigned int>* hashList, int maxVertices=0);
        std::vector<unsigned int> getHashList(Vector<NumCDataType> vector, int maxVertices);
    public:
        HyperCube(int _w=50000): w{_w}, d{-1}, data{NULL}, hashTableSize{0}, hashTable{NULL} {};
        ~HyperCube();

        void fit(NumC<NumCDataType>* _data, int k=-1);
        void transform();
        void fit_transform(NumC<NumCDataType>* _data, int k=-1);
        Results* predict_knn(Vector<NumCDataType> vector, int k, int maxPoints, int maxVertices);
        Results* predict_knn(NumC<NumCDataType>* testData, int k, int maxPoints, int maxVertices);
        Results* predict_rs(Vector<NumCDataType> vector, int r, int maxPoints, int maxVertices);
        std::vector<Results*> predict_rs(NumC<NumCDataType>* testData, int r, int maxPoints, int maxVertices);
        Results* reverse_assignment(NumC<NumCDataType>* centroids, int maxPoints, int maxVertices);
};

template class HyperCube<int>;
template class HyperCube<long>;
template class HyperCube<double>;

#endif