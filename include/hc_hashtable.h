#include "./numc.h";

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
        // List* bucketList;
        int numOfBuckets;
        HC_HashFunction* hashFunction;

    public:
        HC_HashTable(int w, int numOfBucket, int k);
        ~HC_HashTable();

        int hash(Vector vector);
        List getBucket(Vector vector);
        void fit(numC* data);
};