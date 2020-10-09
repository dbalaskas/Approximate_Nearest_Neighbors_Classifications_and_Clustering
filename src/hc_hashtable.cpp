#include "../include/hc_hashtable.h"
#include <time.h>
#include <stdlib.h>

#define R_MULTIPLIER 100

using namespace std;

HC_HashTable::HC_HashTable() {
    hashFunction.w = 0;
    numOfBuckets = 0;
}


HC_HashTable::HC_HashTable(int _r, int _numOfBucket, int _k, int _d) {
    srand(time(NULL));
    hashFunction.w = _r*R_MULTIPLIER;
    hashFunction.sVector = NumC(_k, _d);
    for (int i=0; i < _k; i++) {
        for (int j=0; j < _d; j++) {
            hashFunction.sVector.addElement(rand()%hashFunction.w, i, j);
        }
    }
    bucketList = BucketList(_numOfBucket);
    numOfBuckets = numOfBuckets;
}

HC_HashTable::~HC_HashTable() {
    hashFunction.w = 0;
    numOfBuckets = 0;
}

int HC_HashTable::hash(Vector vector) {
    // ...
}

Bucket HC_HashTable::getBucket(int bucketNum) {
    return bucketList[bucketNum];
}

Bucket HC_HashTable::getBucket(Vector vector) {
    return bucketList[hash(vector)];
}

void HC_HashTable::fit(Vector vector) {
    bucketList[hash(vector)].push_back(vector);
}

void HC_HashTable::fit(NumC* data) {
    int dataSize = data->getRows();
    for (int i=0; i < dataSize; i++) {
        fit(data->getVector(i));
    }
}