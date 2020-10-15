#ifndef LSH_HASH_H
#define LSH_HASH_H

#include <iostream>
#include <vector>
#include "./numc.h"
#include "./hash_function.h"



// template <typename NumCDataType>
// typedef std::vector<Vector<NumCDataType>> Bucket;

// template <typename NumCDataType>


template <typename NumCDataType>
struct Node{
    int index;
    Vector<NumCDataType> sVector;
};


// typedef std::vector< std::vector< Node<NumCDataType> > > BucketList;


template <typename NumCDataType>
class LSH_HashTable {

    private:
        std::vector< std::vector< Node<NumCDataType> > > bucketList;
        int numOfBuckets;
        HashFunction<NumCDataType> hashFunction;
        
    public:
        LSH_HashTable();
        LSH_HashTable(int r, int numOfBuckets, int k, int d);
        ~LSH_HashTable();

        int getNumOfBuckets();
        int hash(Vector<NumCDataType> vector);
        std::vector< Node<NumCDataType> > getBucket(int bucketNum);
        std::vector< Node<NumCDataType> > getBucket(Vector<NumCDataType> vector);
        void fit(Vector<NumCDataType> vector);
        void fit(NumC<NumCDataType>* data);
};

#endif