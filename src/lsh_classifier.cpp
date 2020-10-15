#include "../include/lsh_classifier.h"

#define HASHTABLE_SIZE data->getRows()/8

using namespace std;

template <typename NumCDataType>
LSH<NumCDataType>::LSH(int N, int L, int k, int w)
: data{NULL}, N{N}, L{L}, k{k}, w{w}, hashTableList(L)
{
    for (int i=0; i < L; i++) {
        hashTableList.push_back(HashTable<NumCDataType>(LSH, this->L, this->k, _data->getCols(), this->w));
    }
}


template <typename NumCDataType>
LSH<NumCDataType>::~LSH() {
    data = NULL;
    L = 0;
    N = 0;
    k = 0;
    w = 0;
}

template <typename NumCDataType>
void LSH<NumCDataType>::fit(NumC<NumCDataType>* _data) {
    this->data = _data;

    // if (hashTableSize == 0) {
    //     hashTableSize = HASHTABLE_SIZE;
    // } else {
    //     hashTableSize = _hashTableSize;
    // }

}

template <typename NumCDataType>
void LSH<NumCDataType>::transform() {
    for (int i=0; i < L; i++) {
       hashTableList[i].fit(data); 
    }
}

template <typename NumCDataType>
void LSH<NumCDataType>::fit_transform(NumC<NumCDataType>* _data) {
    fit(_data);
    transform();
}

template <typename NumCDataType>
Results* LSH<NumCDataType>::predict_knn(Vector<NumCDataType> vector, int N) {

    ResultsComparator resultsComparator(this->N);
    vector< Node<NumCDataType> > bucket;

    clock_t start = clock();
    for (int i=0; i < L; i++) {
        // get bucket
        bucket = hashTableList[i].getBucket(vector);

        for (int j=0; j < bucket.size(); j++) {
            resultsComparator.addResult(bucket[j].index, NumC<NumCDataType>::dist(vector, bucket[j].sVector, 1));
            // dist = NumC<NumCDataType>::dist(vector, bucket[j].sVector, 1);
        }
        // results.add(index, dist);
    }
    clock_t end = clock();

    Results* results = resultsComparator.getResults();
    // results 
    results->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);

    return results;
}

template <typename NumCDataType>
Results* LSH<NumCDataType>::predict_knn(NumC<NumCDataType>* testData, int k) {
    // Results result = Results(k);
    // double dist;

    // clock_t start = clock();
    // for (int i=0; i < L; i++) {
    //     Bucket bucket = hashTableList[i].getBucket(vector);

    //     for (int j=0; j < bucket.size(); j++) {
            
    //         dist = NumC<NumCDataType>::dist(vector, bucket[j], 1);
    //     }
    //     // results.add(index, dist);
    // }
    // clock_t end = clock();

    // // result.setTime((double) (end - start) / CLOCKS_PER_SEC);
    // return result;
}

template <typename NumCDataType>
Results* LSH<NumCDataType>::predict_rs(Vector<NumCDataType> vector, int r) {
    // Results result = Results();
    // double dist;
    
    // clock_t start = clock();
    // for (int i=0; i < L; i++) {
    //     Bucket bucket = hashTableList[i].getBucket(vector);
    //     for (int j=0; j < bucket.size(); j++) {
    //         dist = NumC<NumCDataType>::dist(vector, bucket[j], 1);
    //         if (dist <= r) {
    //             // results.add(index, dist);
    //         }
    //     }
    // }
    // clock_t end = clock();

    // // result.setTime((double) (end - start) / CLOCKS_PER_SEC);
    // // result.time = (double)(end - start) / CLOCKS_PER_SEC;
    // return result;
}

#include "../include/pandac.h"
int main(){
    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");

    LSH<int> lsh(1);

}