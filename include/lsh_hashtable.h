#ifndef LSH_HASH_H
#define LSH_HASH_H

#include <iostream>
#include <list>
#include "./numc.h"

class LSH_HashFunction {
    private:
        int w;
        int* sVector;
    public:
        LSH_HashFunction(int w, int k);
        ~LSH_HashFunction();

        int hash(Vector vector);
};

class LSH_HashTable {
    private:
        LSH_HashFunction* hashFunction;
        std::list<Vector>* bucketList;
        int numOfBuckets;

    public:
        LSH_HashTable(int w, int numOfBucket, int k);
        ~LSH_HashTable();

        int hash(Vector vector);
        std::list<Vector> getBucket(Vector vector);
        void fit(NumC* data);
};

#endif