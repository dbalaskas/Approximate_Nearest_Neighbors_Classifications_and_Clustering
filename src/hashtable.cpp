#include "../include/hashtable.h"
#include <time.h>
#include <stdlib.h>

#define R_MULTIPLIER 100
#define W 100

using namespace std;

template <typename NumCDataType>
HashTable<NumCDataType>::HashTable() {
    numOfBuckets = 0;
    hashType = LSH;
}

template <typename NumCDataType>
HashTable<NumCDataType>::HashTable(HashType _hashType, int _numOfBuckets, int k, int d, int w) {
    this->bucketList.reserve(_numOfBuckets);
    this->numOfBuckets = _numOfBuckets;
    this->hashFunction = HashFunction<NumCDataType>(k, d, W);
    hashType = _hashType;
}

template <typename NumCDataType>
HashTable<NumCDataType>::~HashTable() {
    numOfBuckets = 0;
}

template <typename NumCDataType>
int HashTable<NumCDataType>::getNumOfBuckets() {
    return numOfBuckets;
}

template <typename NumCDataType>
char* HashTable<NumCDataType>::getHashType() {
    return HashTypes[hashType];
}

template <typename NumCDataType>
int HashTable<NumCDataType>::hash(Vector<NumCDataType> vector) {
    switch (hashType) {
        case LSH:
            return hashFunction->lsh_hash(vector) % numOfBuckets;
        case HC:
            return hashFunction->hc_hash(vector) % numOfBuckets;
    }
}

template <typename NumCDataType>
std::vector< Node<NumCDataType> > HashTable<NumCDataType>::getBucket(int bucketNum) {
    return bucketList[bucketNum];
}

template <typename NumCDataType>
std::vector< Node<NumCDataType> > HashTable<NumCDataType>::getBucket(Vector<NumCDataType> vector) {
    return bucketList[hash(vector)];
}

template <typename NumCDataType>
void HashTable<NumCDataType>::fit(Vector<NumCDataType> vector) {
    bucketList[hash(vector)].push_back(vector);
}

template <typename NumCDataType>
void HashTable<NumCDataType>::fit(NumC<NumCDataType>* data) {
    int dataSize = data->getRows();
    for (int i=0; i < dataSize; i++) {
        fit(data->getVector(i));
    }
}