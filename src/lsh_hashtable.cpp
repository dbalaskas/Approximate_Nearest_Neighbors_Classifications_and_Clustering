#include "../include/lsh_hashtable.h"
#include <time.h>
#include <stdlib.h>

LSH_HashFunction::LSH_HashFunction() {
    this->w = 0;
}

LSH_HashFunction::LSH_HashFunction(int w, int k, int d) {
    srand(time(NULL));
    this->w = w;
    this->sVector = NumC(k, d);
    for (int i=0; i<d; i++) {
        for (int j=0; j<k; j++) {
            // this->sVector[i, j] = rand()%w;
        }
    }
}

LSH_HashFunction::~LSH_HashFunction() {
    free(this->sVector);
    delete[] this->sVector;
}

int LSH_HashFunction::hash(Vector vector) {
    //...
    return 0;
}
// ---------------------------------------------------------------
LSH_HashTable::LSH_HashTable() {
    numOfBuckets = 0;
}


LSH_HashTable::LSH_HashTable(int w, int numOfBucket, int k, int d) {
    this->hashFunction = LSH_HashFunction(w, k, d);
    this->bucketList = std::vector<std::vector<Vector>>(numOfBucket);
    this->numOfBuckets = numOfBuckets;
}

LSH_HashTable::~LSH_HashTable() {
    numOfBuckets = 0;
}

int LSH_HashTable::hash(Vector vector) {
    return this->hashFunction.hash(vector);
}

std::vector<Vector> LSH_HashTable::getBucket(Vector vector) {
    return this->bucketList[this->hashFunction.hash(vector)];
}

void LSH_HashTable::fit(NumC* data) {
    int dataSize = data->getRows();
    int hashValue;
    Vector vector;
    for (int i=0; i < dataSize; i++) {
        vector = data->getVector(i);
        hashValue = this->hashFunction.hash(vector);
        this->bucketList[i].push_back(vector);
    }
}