#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include "./numc.h"
#include "./hash_function.h"

// Enumeration of hashFunction methods. HashTable's type.
enum HashType {LSH, HC};

// Define the bucketlist node with the Vector, its index on train data and its hash value.
template <typename NumCDataType>
struct Node{
    int index;                      // index of Vector in train data.
    Vector<NumCDataType> sVector;   // Vector in bucket.
    unsigned int hashValue;         // hash value of vector.
};



template <typename NumCDataType>
class HashTable {

    private:
        std::vector< std::vector< Node<NumCDataType> > > bucketList; // bucketlist
        int numOfBuckets;                                            // number of buckets
        HashFunction<NumCDataType> hashFunction;                     // hashFunction's object
        HashType hashType;                                           // Type of hashTable
        
    public:
        HashTable(HashType _hashType=LSH): numOfBuckets{0}, hashType{_hashType} {};
        HashTable(HashType _hashType, int _numOfBuckets, int k, int d, int w);
        ~HashTable();

        // Returns hashTable's number of buckets.
        int getNumOfBuckets();
        // Returns a string that represents the hashTable's type.
        const char* getHashType(HashType hashType);
        // Computes and returns hash value of 'vector'.
        unsigned int hash(Vector<NumCDataType> vector);
        // Get bucket based on hashValue.
        std::vector< Node<NumCDataType> > getBucket(unsigned int bucketNum);
        // Get bucket of 'vector'.
        std::vector< Node<NumCDataType> > getBucket(Vector<NumCDataType> vector);
        // imports 'vector' in table.
        void fit(Vector<NumCDataType> vector, int index);
        // imports 'data' matrix in table. Using the previous method for each row.
        void fit(NumC<NumCDataType>* data);
};

// Define the templates of Hypercube
template class HashTable<int>;
template class HashTable<long>;
template class HashTable<double>;


#endif