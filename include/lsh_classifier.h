#ifndef LSH_H
#define LSH_H

#include <vector>
#include "./numc.h"
#include "./hashtable.h"
#include "./prediction_results.h"
#include "./exhaustive_knn.h"

// typedef vector<HashTable<NumCDataType>> HashTableList;


template <typename NumCDataType>
class LSHashing {

    private:
        int L;                                      // size of hashTable list
        int k;                                      // number of 'hi' that will be used on hashFunction
        int w;                                      // w for hashFunction
        NumC<NumCDataType>* data;                   // train data
        HashTable<NumCDataType>** hashTableList;    // list of hashTables

    public:
        LSHashing(): L{5}, k{4}, w{10}, data{NULL}, hashTableList{NULL} {};
        // Initializes hash Table list with size of k.
        LSHashing(int L = 5, int k = 4, int w = 50000);
        ~LSHashing();

        // Defines data and initializes hashTables with size based on k.
        void fit(NumC<NumCDataType>* _data);
        // Trains classifier by importing data to hashtables.
        void transform();
        // Fits and transform classifier.
        void fit_transform(NumC<NumCDataType>* _data);

        // Predicts the k-NN of the given vector.
        Results* predict_knn(Vector<NumCDataType> vector, int N);
        // Predicts the k-NN of the given matrix. Calls the previous method for each row.
        Results* predict_knn(NumC<NumCDataType>* testData, int N);

        // Executes range search of the given vector.
        Results* predict_rs(Vector<NumCDataType> vector, double r);
        // Executes range search of the given matrix. Calls the previous method for each row.
        std::vector<Results*> predict_rs(NumC<NumCDataType>* testData, int r);

        // Reverse assignment implementation that is used on Kmedians.
        Results* reverse_assignment(NumC<NumCDataType>* centroids);
};

// Define the templates of Hypercube
template class LSHashing<int>;
template class LSHashing<long>;
template class LSHashing<double>;

#endif