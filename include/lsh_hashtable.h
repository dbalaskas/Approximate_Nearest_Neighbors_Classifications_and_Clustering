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
        // LSH_HashFunction hashFunction;
        std::vector<std::vector<Vector>> bucketList;
        int numOfBuckets;

    public:
        LSH_HashTable();
        LSH_HashTable(int w, int numOfBucket, int k, int d);
        ~LSH_HashTable();

        int hash(Vector vector);
        std::vector<Vector> getBucket(Vector vector);
        void fit(NumC* data);
        void fit(Vector data);
};

#endif