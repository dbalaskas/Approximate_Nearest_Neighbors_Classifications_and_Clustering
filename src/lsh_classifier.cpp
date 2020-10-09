#include "../include/lsh_classifier.h"

#define HASHTABLE_SIZE data->getRows()/8

using namespace std;

LSH::LSH() {
    data = NULL;
    hashTableSize = 0;
    L = 0;
    R = 0;
}

LSH::~LSH() {
    data = NULL;
    hashTableSize = 0;
    L = 0;
    R = 0;
}

void LSH::fit(NumC* _data, int k, int _R, int _L, int _hashTableSize=0) {
    data = _data;
    R = _R;
    if (hashTableSize == 0) {
        hashTableSize = HASHTABLE_SIZE;
    } else {
        hashTableSize = _hashTableSize;
    }
    hashTableList = HashTableList(hashTableSize);
    for (int i=0; i < L; i++) {
        hashTableList[i] = LSH_HashTable(R, hashTableSize, k, data->getCols());
    }
}

void LSH::transform() {
    for (int i=0; i < L; i++) {
       hashTableList[i].fit(data); 
    }
}

void LSH::fit_transform(NumC* _data, int k, int _R, int _L, int _hashTableSize=0) {
    fit(_data, k, _R, _L, _hashTableSize);
    transform();
}

Result LSH::predict_knn(Vector vector, int k) {
    Result result;
    for (int i=0; i < L; i++) {
    //    hashTableList[i].fit(data);
        double dist;
        Bucket bucket = hashTableList[i].getBucket(vector);
        for (int j=0; j < bucket.size(); j++) {
            dist = NumC::dist(vector, bucket[j], 1);
        }
    }
    return result;
}

Result LSH::predict_rs(Vector vector, int r) {
    Result result;
    
    return result;
}
