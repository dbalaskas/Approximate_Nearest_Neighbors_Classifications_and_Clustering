#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include "./numc.h"
#include "./hash_function.h"



// template <typename NumCDataType>
// typedef std::vector<Vector<NumCDataType>> Bucket;
// typedef std::vector< std::vector< Node<NumCDataType> > > BucketList;

enum HashType {LSH, HC};
const char* HashTypes[] = {"LSH", "HC"};

template <typename NumCDataType>
struct Node{
    int index;
    Vector<NumCDataType> sVector;
};




template <typename NumCDataType>
class HashTable {

    private:
        std::vector< std::vector< Node<NumCDataType> > > bucketList;
        int numOfBuckets;
        HashFunction<NumCDataType> hashFunction;
        HashType hashType;
        
    public:
        HashTable();
        HashTable(HashType _hashType, int _numOfBuckets, int k, int d, int w);
        ~HashTable();

        int getNumOfBuckets();
        char* getHashType();
        int hash(Vector<NumCDataType> vector);
        std::vector< Node<NumCDataType> > getBucket(int bucketNum);
        std::vector< Node<NumCDataType> > getBucket(Vector<NumCDataType> vector);
        void fit(Vector<NumCDataType> vector);
        void fit(NumC<NumCDataType>* data);
};

#endif