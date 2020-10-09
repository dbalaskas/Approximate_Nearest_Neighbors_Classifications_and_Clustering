#ifndef LSH_HASH_H
#define LSH_HASH_H

#include <iostream>
#include <vector>
#include "./numc.h"

typedef std::vector<std::vector<Vector>> BucketList;
typedef std::vector<Vector> Bucket;
typedef struct {
    int w;
    NumC sVector;
} HashFunction;

class LSH_HashTable {
    private:
        HashFunction hashFunction;
        BucketList bucketList;
        int numOfBuckets;

    public:
        LSH_HashTable();
        LSH_HashTable(int r, int numOfBuckets, int k, int d);
        ~LSH_HashTable();

        int getNumOfBuckets();
        int hash(Vector vector);
        Bucket getBucket(int bucketNum);
        Bucket getBucket(Vector vector);
        void fit(Vector vector);
        void fit(NumC* data);
};

#endif