#include "../include/lsh_hashtable.h"
#include <time.h>
#include <stdlib.h>

using namespace std;

// LSH_HashFunction::LSH_HashFunction() {
//     this->w = 0;
// }

// LSH_HashFunction::LSH_HashFunction(int w, int k, int d) {
//     srand(time(NULL));
//     this->w = w;
//     this->sVector = NumC(k, d);
//     for (int i=0; i<d; i++) {
//         for (int j=0; j<k; j++) {
//             this->sVector.addElement(rand()%w, i, j);
//         }
//     }
// }

// LSH_HashFunction::~LSH_HashFunction() {
//     this->w = 0;
// }

// int LSH_HashFunction::hash(Vector vector) {
//     //...
//     return 0;
// }

LSH_HashTable::LSH_HashTable() {
    hashFunction.w = 0;
    numOfBuckets = 0;
}


LSH_HashTable::LSH_HashTable(int _w, int _numOfBucket, int _k, int _d) {
    srand(time(NULL));
    hashFunction.w = _w;
    hashFunction.sVector = NumC(_k, _d);
    for (int i=0; i<_d; i++) {
        for (int j=0; j<_k; j++) {
            hashFunction.sVector.addElement(rand()%_w, i, j);
        }
    }
    // hashFunction = LSH_HashFunction(w, k, d);
    bucketList = vector<vector<Vector>>(_numOfBucket);
    numOfBuckets = numOfBuckets;
}

LSH_HashTable::~LSH_HashTable() {
    hashFunction.w = 0;
    numOfBuckets = 0;
}

int LSH_HashTable::hash(Vector vector) {
    // return this->hashFunction.hash(vector);
    // ...
}

std::vector<Vector> LSH_HashTable::getBucket(Vector vector) {
    // return this->bucketList[this->hashFunction.hash(vector)];
    return bucketList[hash(vector)];
}

void LSH_HashTable::fit(Vector vector) {
    bucketList[hash(vector)].push_back(vector);
}

void LSH_HashTable::fit(NumC* data) {
    int dataSize = data->getRows();
    for (int i=0; i < dataSize; i++) {
        fit(data->getVector(i));
    }
}