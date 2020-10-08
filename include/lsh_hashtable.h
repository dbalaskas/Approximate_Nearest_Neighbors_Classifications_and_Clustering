#ifndef LSH_HASH_H
#define LSH_HASH_H

#include <iostream>
#include <vector>
#include "./numc.h"

class LSH_HashFunction {
    private:
        int w;
        NumC sVector;
    public:
        LSH_HashFunction();
        LSH_HashFunction(int w, int k, int d);
        ~LSH_HashFunction();

        int hash(Vector vector);
};

class LSH_HashTable {
    private:
        LSH_HashFunction hashFunction;
        std::vector<std::vector<Vector>> bucketList;
        int numOfBuckets;

    public:
        LSH_HashTable();
        LSH_HashTable(int w, int numOfBucket, int k, int d);
        ~LSH_HashTable();

        int hash(Vector vector);
        std::vector<Vector> getBucket(Vector vector);
        void fit(NumC* data);
};

#endif