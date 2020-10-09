#include "../include/hc_hashtable.h"
#include <time.h>
#include <stdlib.h>

using namespace std;

HC_HashTable::HC_HashTable() {
    hashFunction.w = 0;
    numOfBuckets = 0;
}


HC_HashTable::HC_HashTable(int _w, int _numOfBucket, int _k, int _d) {
    srand(time(NULL));
    hashFunction.w = _w;
    hashFunction.sVector = NumC(_k, _d);
    for (int i=0; i<_d; i++) {
        for (int j=0; j<_k; j++) {
            hashFunction.sVector.addElement(rand()%_w, i, j);
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