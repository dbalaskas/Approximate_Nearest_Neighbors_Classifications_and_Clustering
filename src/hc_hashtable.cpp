#include "../include/hc_hashtable.h"
#include <time.h>
#include <stdlib.h>

#define R_MULTIPLIER 100

using namespace std;

HC_HashTable::HC_HashTable() {
    cout << "DEFAULT CONSTRUCTOR" << endl;
    hashFunction.w = 0;
    // hashFunction.sVector = NULL;
    numOfBuckets = 0;
}


HC_HashTable::HC_HashTable(int _r, int _numOfBuckets, int _k, int _d) {
    cout << "NOT DEFAULT" << endl;
    srand(time(NULL));
    hashFunction.w = _r*R_MULTIPLIER;
    // hashFunction.sVector = (NumC*) malloc(sizeof(NumC));
    // *hashFunction.sVector = NumC(_k, _d);
    hashFunction.sVector = NumC(_k, _d);
    for (int i=0; i < _k; i++) {
        for (int j=0; j < _d; j++) {
            hashFunction.sVector.addElement(rand()%hashFunction.w, i, j);
        }
    }
    bucketList = BucketList(_numOfBuckets);
    numOfBuckets = _numOfBuckets;
}

HC_HashTable::~HC_HashTable() {
    cout << "DESTRUCTED" << endl;
    // cout << hashFunction.sVector << endl;
    cout << numOfBuckets << endl;
    hashFunction.w = 0;
    // free(hashFunction.sVector);
    // hashFunction.sVector = NULL;
    numOfBuckets = 0;
    cout << "END DESTRUCTED" << endl;
}

int HC_HashTable::getNumOfBuckets() {
    // cout << "numpy" << hashFunction.sVector << endl;
    // cout << "numofbuckets" << numOfBuckets << endl;
    return numOfBuckets;
}

int HC_HashTable::hash(Vector vector) {
    // Computr HyperCube hash function.
    return 1;
}

Bucket HC_HashTable::getBucket(int bucketNum) {
    return bucketList[bucketNum];
}

Bucket HC_HashTable::getBucket(Vector vector) {
    return bucketList[hash(vector)];
}

void HC_HashTable::fit(Vector vector) {
    bucketList[hash(vector)].push_back(vector);
}

void HC_HashTable::fit(NumC* data) {
    int dataSize = data->getRows();
    for (int i=0; i < dataSize; i++) {
        fit(data->getVector(i));
    }
}

// ------------------------------------------------
// Test
// ------------------------------------------------

int main() {
    HC_HashTable hc;
    hc = HC_HashTable(1, 10, 3, 5);
    // HC_HashTable hc = HC_HashTable(1, 10, 3, 5);
    cout << "starting" << endl;
    BucketList bucket = BucketList(10);

    NumC nn(6,33);
    for (int i = 0; i < nn.getRows(); i++){
        for (int j = 0; j < nn.getCols(); j++){
            nn.addElement(i, i, j);
        }
        
    }
    nn.print();
    cout << "OK1" << endl << endl;


    // Test HashTable with <void fit(Vector data)>
    Vector v = nn.getVector(3);
    NumC::print(v);
    hc.fit(v);
    cout << hc.getBucket(1).size() << endl;
    // bucket[1].push_back(v);
    v = nn.getVector(1);
    NumC::print(v);
    hc.fit(v);
    cout << hc.getBucket(1).size() << endl;
    // bucket[1].push_back(v);
    v = nn.getVector(4);
    NumC::print(v);
    hc.fit(v);
    cout << hc.getBucket(1).size() << endl;
    // bucket[1].push_back(v);
    // bucket[1].push_back(v);
    // bucket[1].push_back(v);
    // for(int i=0; i < hc.getNumOfBuckets(); i++) {
    //     cout << hc.getBucket(i).size() << endl;
    // }
    cout << "OK2" << endl;

    // Harder Test HashTable with <void fit(Vector data)>
    v = nn.getVector(1);
    hc.fit(v);
    v = nn.getVector(2);
    hc.fit(v);
    v = nn.getVector(3);
    hc.fit(v);
    v = nn.getVector(4);
    hc.fit(v);
    v = nn.getVector(1);
    hc.fit(v);
    // for(int i=0; i < hc.getNumOfBuckets(); i++) {
    //     cout << hc.getBucket(i).size() << endl;
    // }

    // Test HashTable with <void fit(NumC* data)>
    cout << endl;
    hc.fit(&nn);
    // for(int i=0; i < hc.getNumOfBuckets(); i++) {
    //     cout << hc.getBucket(i).size() << endl;
    // } //13

    cout << endl << "Finishing all OK!" << endl;
    return 0;
}