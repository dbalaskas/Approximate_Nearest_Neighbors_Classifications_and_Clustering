#include <time.h>
#include <stdlib.h>
#include "../include/hashtable.h"

#define R_MULTIPLIER 100
#define W 100

using namespace std;

template <typename NumCDataType>
HashTable<NumCDataType>::HashTable(HashType _hashType, int _numOfBuckets, int k, int d, int w)
: bucketList(_numOfBuckets), numOfBuckets{_numOfBuckets}, hashFunction(k, d, w), hashType{_hashType}
{}

template <typename NumCDataType>
HashTable<NumCDataType>::~HashTable() {
    numOfBuckets = 0;
}

template <typename NumCDataType>
int HashTable<NumCDataType>::getNumOfBuckets() {
    return numOfBuckets;
}

template <typename NumCDataType>
const char* HashTable<NumCDataType>::getHashType(HashType hashType) {
    switch (hashType) {
        case LSH:
            return "LSH";
        case HC:
            return "HC";
        default:
            return "ERROR";
    }
}

template <typename NumCDataType>
unsigned int HashTable<NumCDataType>::hash(Vector<NumCDataType> vector) {
    switch (hashType) {
        case LSH:
            return hashFunction.lsh_hash(vector);
        case HC:
            return hashFunction.hc_hash(vector);
        default:
            return 0;
    }
}

// template <typename NumCDataType>
// unsigned int HashTable<NumCDataType>::get_hammingDist(Vector<NumCDataType> vector, int i, int changesLeft, vector<unsigned int>* hashList, int numOfElements=0) {
//     unsigned int hashValue = hash(vector);
//     if (hashList->size() == numOfElements) return;
//     if (changeLeft == 0) {
//         hashList->pushBack(hashValue);
//         numOfElements++;
//         return;
//     }
//     if (i<0) return;
//     mask = 1 << i-1;
//     get_hammingDist(vector^mask, i-1, changesLeft-1, hashList, numOfElements);
//     get_hammingDist(vector, i-1, changesLeft, hashList, numOfElements);
//     // unsigned int mask = 0;
//     // unsigned int start_pos = 0;
//     // unsigned int steps = hashFunction.getk();
//     // cout << numOfBuckets << " vs " << i << endl;
//     // if (numOfBuckets <= i) return -1;
//     // while (i > steps && steps > 0) {
//     //     start_pos++;
//     //     mask = (mask << 1) + 1;
//     //     i -= steps;
//     //     steps --;
//     // }
//     // cout << "continue aces until: " << start_pos << " and one ace at: " << i << endl;
//     // if (i > 0)
//     //     mask |= (1 << start_pos+i-1);
//     // hashValue = hashValue ^ mask;

//     return hashValue;
// }

template <typename NumCDataType>
vector< Node<NumCDataType> > HashTable<NumCDataType>::getBucket(unsigned int bucketNum) {
    return bucketList[bucketNum];
}

template <typename NumCDataType>
vector< Node<NumCDataType> > HashTable<NumCDataType>::getBucket(Vector<NumCDataType> vector) {
    return bucketList[hash(vector)%numOfBuckets];
}

template <typename NumCDataType>
void HashTable<NumCDataType>::fit(Vector<NumCDataType> vector, int index) {
    struct Node<NumCDataType> node;
    node.index = index;
    node.sVector = vector;
    node.hashValue = hash(vector);
    bucketList[node.hashValue % (unsigned int)numOfBuckets].push_back(node);
}

template <typename NumCDataType>
void HashTable<NumCDataType>::fit(NumC<NumCDataType>* data) {
    int dataSize = data->getRows();
    for (int i=0; i < dataSize; i++) {
        fit(data->getVector(i), i);
    }
}

// #include "../include/pandac.h"
// #include <math.h>
// int main(){


//     // NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
//     // // NumC<int>::print(inputData->getVector(0));
//     // // NumC<int>::printSparse(inputData->getVector(1));


//     // NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");
//     // // NumC<int>::print(inputDatalabels->getVector(0));


//     // hash.h(inputData->getVector(0), 0);
//     // hash.h(inputData->getVector(0), 1);
//     // hash.h(inputData->getVector(0), 2);
//     // hash.h(inputData->getVector(0), 3);
//     // hash.h(inputData->getVector(0), 4);

//     // for (int i = 0; i < 10; i++){
//     //     cout << "lshHash-> " << hash.lsh_hash(inputData->getVector(i)) <<endl;
//     // }
    // HashTable<int> table(HC, 512, log2(1024)-1, 28*28, 10);

//     // table.fit(inputData);

//     // for (int i = 0; i < table.getNumOfBuckets(); i+=10){
//     //     cout << table.getBucket(i).size() << " " << 
//     //     table.getBucket(i+1).size() << " " << 
//     //     table.getBucket(i+2).size() << " " << 
//     //     table.getBucket(i+3).size() << " " << 
//     //     table.getBucket(i+4).size() << " " << 
//     //     table.getBucket(i+5).size() << " " <<
//     //     table.getBucket(i+6).size() << " " <<
//     //     table.getBucket(i+7).size() << " " <<
//     //     table.getBucket(i+8).size() << " " <<
//     //     table.getBucket(i+9).size() << " " <<
//     //     table.getBucket(i+10).size() << " " <<endl;
//     // }
//     // for (int i = 0; i < table.getNumOfBuckets(); i+=1){
//     //     cout << table.getBucket(i).size() << " ";
//     //     if (i > 0 && i%10 == 0) cout << endl;
//     // }

//     //     cout << endl << table.getNumOfBuckets() << endl;
    

    // delete inputData;

// }