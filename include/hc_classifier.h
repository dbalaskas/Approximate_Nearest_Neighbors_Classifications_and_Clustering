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
        int w;                                  // w for hashFunction
        int d;                                  // d for hashFunction
        NumC<NumCDataType>* data;               // train data
        int hashTableSize;                      // size of HashTable
        HashTable<NumCDataType>* hashTable;     // HashTable

        // Gets nearest hashed based on Hamming distanse.
        void get_nearestHashes(unsigned int vector, int k, int changesLeft, std::vector<unsigned int>* hashList, int maxVertices=0);
        // Gets a list with, minimum, <maxVertices> nearest values, based on Hamming distance.
        std::vector<unsigned int> getHashList(Vector<NumCDataType> vector, int maxVertices);
    public:
        HyperCube(int _w=50000): w{_w}, data{NULL}, hashTableSize{0}, hashTable{NULL} {};
        ~HyperCube();

        // Defines data and initializes hashTable with size based on k. if k ==-1 then size becomes equal to log2(data->getRows())-1.
        void fit(NumC<NumCDataType>* _data, int k=-1);
        // Trains classifier by importing data to hashtable.
        void transform();
        // Fits and transform classifier.
        void fit_transform(NumC<NumCDataType>* _data, int k=-1);

        // Predicts the k-NN of the given vector, based on maxVertices and maxPoints.
        Results* predict_knn(Vector<NumCDataType> vector, int k, int maxPoints, int maxVertices);
        // Predicts the k-NN of the given matrix, based on maxVertices and maxPoints. Calls the previous method for each row.
        Results* predict_knn(NumC<NumCDataType>* testData, int k, int maxPoints, int maxVertices);
        
        // Executes range search of the given vector, based on maxVertices and maxPoints.
        Results* predict_rs(Vector<NumCDataType> vector, int r, int maxPoints, int maxVertices);
        // Executes range search of the given matrix, based on maxVertices and maxPoints. Calls the previous method for each row.
        std::vector<Results*> predict_rs(NumC<NumCDataType>* testData, int r, int maxPoints, int maxVertices);
        
        // Reverse assignment implementation that is used on Kmedians.
        Results* reverse_assignment(NumC<NumCDataType>* centroids, int maxPoints, int maxVertices);
};

// Define the templates of Hypercube
template class HyperCube<int>;
template class HyperCube<long>;
template class HyperCube<double>;

#endif