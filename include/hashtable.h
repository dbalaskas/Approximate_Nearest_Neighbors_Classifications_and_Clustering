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
// extern const char* HashTypes[] = {"LSH", "HC"};

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
        HashTable(HashType _hashType=LSH): numOfBuckets{0}, hashType{_hashType} {};
        HashTable(HashType _hashType, int _numOfBuckets, int k, int d, int w);
        ~HashTable();

        int getNumOfBuckets();
        const char* getHashType(HashType hashType);
        int hash(Vector<NumCDataType> vector);
        std::vector< Node<NumCDataType> > getBucket(unsigned int bucketNum);
        std::vector< Node<NumCDataType> > getBucket(Vector<NumCDataType> vector);
        void fit(Vector<NumCDataType> vector, int index);
        void fit(NumC<NumCDataType>* data);
};

template class HashTable<int>;
template class HashTable<long>;
template class HashTable<double>;


#endif