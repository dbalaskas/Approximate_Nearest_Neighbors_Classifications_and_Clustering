#include "../include/lsh_hashtable.h"
// #include "../include/hash_function.h"
#include <time.h>
#include <stdlib.h>

#define R_MULTIPLIER 100
#define W 100

using namespace std;

template <typename NumCDataType>
LSH_HashTable<NumCDataType>::LSH_HashTable() {
    hashFunction.w = 0;
    numOfBuckets = 0;
}

template <typename NumCDataType>
LSH_HashTable<NumCDataType>::LSH_HashTable(int _r, int _numOfBuckets, int _k, int _d) {
//     srand(time(NULL));
//     hashFunction.w = _r*R_MULTIPLIER;
//     hashFunction.sVector = NumC(_k, _d);
//     for (int i=0; i < _k; i++) {
//         for (int j=0; j < _d; j++) {
//             hashFunction.sVector.addElement(rand()%hashFunction.w, i, j);
//         }
//     }
    // bucketList = BucketList(_numOfBuckets);
    this->bucketList.reserve(_numOfBuckets);
    this->numOfBuckets = _numOfBuckets;
    this->hashFunction = HashFunction(_k, _d, W);

}

template <typename NumCDataType>
LSH_HashTable<NumCDataType>::~LSH_HashTable() {
    numOfBuckets = 0;
}

template <typename NumCDataType>
int LSH_HashTable<NumCDataType>::getNumOfBuckets() {
    return numOfBuckets;
}

template <typename NumCDataType>
int LSH_HashTable<NumCDataType>::hash(Vector<NumCDataType> vector) {
    
    // get h hashes
    

}

template <typename NumCDataType>
std::vector< Node<NumCDataType> > LSH_HashTable<NumCDataType>::getBucket(int bucketNum) {
    return bucketList[bucketNum];
}

template <typename NumCDataType>
std::vector< Node<NumCDataType> > LSH_HashTable<NumCDataType>::getBucket(Vector<NumCDataType> vector) {
    return bucketList[hash(vector)];
}

template <typename NumCDataType>
void LSH_HashTable<NumCDataType>::fit(Vector<NumCDataType> vector) {
    bucketList[hash(vector)].push_back(vector);
}

template <typename NumCDataType>
void LSH_HashTable<NumCDataType>::fit(NumC<NumCDataType>* data) {
    int dataSize = data->getRows();
    for (int i=0; i < dataSize; i++) {
        fit(data->getVector(i));
    }
}