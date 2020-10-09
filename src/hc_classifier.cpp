#include "../include/hc_classifier.h"

#define HASHTABLE_SIZE data->getRows()/8

using namespace std;

HyperCube::HyperCube() {
    data = NULL;
    hashTableSize = 0;
    R = 0;
}

HyperCube::HyperCube() {
    data = NULL;
    hashTableSize = 0;
    R = 0;
}

void HyperCube::fit(NumC* _data, int k, int _R, int _hashTableSize) {
    data = _data;
    R = _R;
    if (hashTableSize == 0) {
        hashTableSize = HASHTABLE_SIZE;
    } else {
        hashTableSize = _hashTableSize;
    }
    hashTable = HC_HashTable(R, hashTableSize, k, data->getCols());
}

void HyperCube::transform() {
    
}

void HyperCube::fit_transform(NumC* _data, int k, int _R, int _hashTableSize) {
    fit(_data, k, _R, _hashTableSize);
    transform();
}

Result HyperCube::predict_knn(Vector vector, int k) {
    Result result;
    
    return result;
}

Result HyperCube::predict_rs(Vector vector, int r) {
    Result result;
    
    return result;
}
