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
    hashTable.fit(data);
}

void HyperCube::fit_transform(NumC* _data, int k, int _R, int _hashTableSize) {
    fit(_data, k, _R, _hashTableSize);
    transform();
}

Results HyperCube::predict_knn(Vector vector, int k, int maxPoints, int maxVertices) {
    Results result = Results(k);
    double dist;
    int verticesProbed = 0;
    int pointesChecked = 0;

    clock_t start = clock();
    int hashValue = hashTable.hash(vector);
    while (verticesProbed < maxVertices && pointesChecked < maxPoints) {
        Bucket bucket = hashTable.getBucket(hashValue);
        for (int j=0; j < bucket.size(); j++) {
            dist = NumC::dist(vector, bucket[j], 1);
            // results.add(index, dist);
            if (++pointesChecked == maxPoints) break;
        }
        // hashValue = Update_hashValue(hashValue)
        verticesProbed++;
    }
    clock_t end = clock();

    // result.setTime((double) (end - start) / CLOCKS_PER_SEC);
    return result;
}

Results HyperCube::predict_rs(Vector vector, int r, int maxPoints, int maxVertices) {
    Results result = Results();
    double dist;
    int verticesProbed = 0;
    int pointesChecked = 0;

    clock_t start = clock();
    int hashValue = hashTable.hash(vector);
    while (verticesProbed < maxVertices && pointesChecked < maxPoints) {
        Bucket bucket = hashTable.getBucket(hashValue);
        for (int j=0; j < bucket.size(); j++) {
            dist = NumC::dist(vector, bucket[j], 1);
            if (dist <= r) {
                // results.add(index, dist);
            }
            if (++pointesChecked == maxPoints) break;
        }
        // hashValue = Update_hashValue(hashValue)
        verticesProbed++;
    }
    clock_t end = clock();

    // result.setTime((double) (end - start) / CLOCKS_PER_SEC);
    // result.time = (double)(end - start) / CLOCKS_PER_SEC;
    return result;
}
