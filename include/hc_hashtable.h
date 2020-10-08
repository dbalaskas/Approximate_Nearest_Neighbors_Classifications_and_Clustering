#ifndef HC_HASH_H
#define HC_HASH_H

#include <iostream>
#include <list>
#include "./numc.h"

class HC_HashFunction {
    private:
        int w;
        int* sVector;
    public:
        HC_HashFunction(int w, int k);
        ~HC_HashFunction();

        int hash(Vector vector);
};

class HC_HashTable {
    private:
        HC_HashFunction* hashFunction;
        std::list<Vector>* bucketList;
        int numOfBuckets;

    public:
        HC_HashTable(int w, int numOfBucket, int k);
        ~HC_HashTable();

        int hash(Vector vector);
        std::list<Vector> getBucket(Vector vector);
        void fit(NumC* data);
};

#endif