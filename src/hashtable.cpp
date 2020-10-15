#include "../include/hashtable.h"
#include <time.h>
#include <stdlib.h>

#define R_MULTIPLIER 100
#define W 100

using namespace std;

template <typename NumCDataType>
HashTable<NumCDataType>::HashTable(HashType _hashType, int _numOfBuckets, int k, int d, int w)
: hashFunction(k, d, w), numOfBuckets{_numOfBuckets}, hashType{_hashType}, bucketList(_numOfBuckets)
{}

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
            return (int)(hashFunction.lsh_hash(vector) % (unsigned int)numOfBuckets);
        case HC:
            return (int)(hashFunction.hc_hash(vector) % (unsigned int)numOfBuckets);
    }
}

template <typename NumCDataType>
vector< Node<NumCDataType> > HashTable<NumCDataType>::getBucket(int bucketNum) {
    return bucketList[bucketNum];
}

template <typename NumCDataType>
vector< Node<NumCDataType> > HashTable<NumCDataType>::getBucket(Vector<NumCDataType> vector) {
    return bucketList[hash(vector)];
}

template <typename NumCDataType>
void HashTable<NumCDataType>::fit(Vector<NumCDataType> vector, int index) {
    struct Node<NumCDataType> node;
    node.index = index;
    node.sVector = vector;
    bucketList[hash(vector)].push_back(node);
}

template <typename NumCDataType>
void HashTable<NumCDataType>::fit(NumC<NumCDataType>* data) {
    int dataSize = data->getRows();
    for (int i=0; i < dataSize; i++) {
        fit(data->getVector(i), i);
    }
}

#include "../include/pandac.h"
int main(){


    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
    // // NumC<int>::print(inputData->getVector(0));
    // // NumC<int>::printSparse(inputData->getVector(1));


    // NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");
    // // NumC<int>::print(inputDatalabels->getVector(0));


    // hash.h(inputData->getVector(0), 0);
    // hash.h(inputData->getVector(0), 1);
    // hash.h(inputData->getVector(0), 2);
    // hash.h(inputData->getVector(0), 3);
    // hash.h(inputData->getVector(0), 4);

    // for (int i = 0; i < 10; i++){
    //     cout << "lshHash-> " << hash.lsh_hash(inputData->getVector(i)) <<endl;
    // }
    HashTable<int> table(LSH, 60000/8, 5, 28*28, 10);

    table.fit(inputData);

    delete inputData;

}