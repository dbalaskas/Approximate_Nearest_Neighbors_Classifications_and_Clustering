#include "./numc.h";

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
        List* bucketList;
        int numOfBuckets;
        LSH_HashFunction* hashFunction;

    public:
        LSH_HashTable(int w, int numOfBucket, int k);
        ~LSH_HashTable();

        int hash(Vector vector);
        List getBucket(Vector vector);
        void fit(numC* data);
};