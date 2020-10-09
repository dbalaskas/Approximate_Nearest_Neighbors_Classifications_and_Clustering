#ifndef HC_HASH_H
#define HC_HASH_H

#include <iostream>
#include <vector>
#include "./numc.h"

typedef std::vector<std::vector<Vector>> BucketList;
typedef std::vector<Vector> Bucket;
typedef struct {
    int w;
    NumC sVector;
} HashFunction;

class HC_HashTable {
    private:
        HashFunction hashFunction;
        BucketList bucketList;
        int numOfBuckets;

    public:
        HC_HashTable();
        HC_HashTable(int r, int numOfBuckets, int k, int d);
        ~HC_HashTable();

        int getNumOfBuckets();
        int hash(Vector vector);
        Bucket getBucket(int bucketNum);
        Bucket getBucket(Vector vector);
        void fit(Vector vector);
        void fit(NumC* data);
};

#endif