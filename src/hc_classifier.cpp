#include <math.h>
#include "../include/hc_classifier.h"

#define HASH_SIZE (log2(data->getRows())-1)

using namespace std;

template <typename NumCDataType>
HyperCube<NumCDataType>::~HyperCube() {
    data = NULL;
    hashTableSize = 0;
    k = 0;
}

template <typename NumCDataType>
void HyperCube<NumCDataType>::fit(NumC<NumCDataType>* _data) {
    data = _data;
    hashTableSize = 0;
    hashTable = HashTable<NumCDataType>(HC, 1<<(int)HASH_SIZE, HASH_SIZE, data->getCols(), 10);
    hashTable.fit(data);
}

template <typename NumCDataType>
void HyperCube<NumCDataType>::transform() {
    hashTable.fit(data);
}

template <typename NumCDataType>
void HyperCube<NumCDataType>::fit_transform(NumC<NumCDataType>* _data) {
    fit(_data);
    transform();
}

// template <typename NumCDataType>
// Results HyperCube<NumCDataType>::predict_knn(Vector<NumCDataType> vector, int k, int maxPoints, int maxVertices) {
//     Results result = Results(k);
//     double dist;
//     int verticesProbed = 0;
//     int pointesChecked = 0;

//     clock_t start = clock();
//     int hashValue = hashTable.hash(vector);
//     while (verticesProbed < maxVertices && pointesChecked < maxPoints) {
//         Bucket bucket = hashTable.getBucket(hashValue);
//         for (int j=0; j < bucket.size(); j++) {
//             dist = NumC::dist(vector, bucket[j], 1);
//             // results.add(index, dist);
//             if (++pointesChecked == maxPoints) break;
//         }
//         // hashValue = Update_hashValue(hashValue)
//         verticesProbed++;
//     }
//     clock_t end = clock();

//     // result.setTime((double) (end - start) / CLOCKS_PER_SEC);
//     return result;
// }

// template <typename NumCDataType>
// Results HyperCube<NumCDataType>::predict_rs(Vector<NumCDataType> vector, int r, int maxPoints, int maxVertices) {
//     Results result = Results();
//     double dist;
//     int verticesProbed = 0;
//     int pointesChecked = 0;

//     clock_t start = clock();
//     int hashValue = hashTable.hash(vector);
//     while (verticesProbed < maxVertices && pointesChecked < maxPoints) {
//         Bucket bucket = hashTable.getBucket(hashValue);
//         for (int j=0; j < bucket.size(); j++) {
//             dist = NumC::dist(vector, bucket[j], 1);
//             if (dist <= r) {
//                 // results.add(index, dist);
//             }
//             if (++pointesChecked == maxPoints) break;
//         }
//         // hashValue = Update_hashValue(hashValue)
//         verticesProbed++;
//     }
//     clock_t end = clock();

//     // result.setTime((double) (end - start) / CLOCKS_PER_SEC);
//     // result.time = (double)(end - start) / CLOCKS_PER_SEC;
//     return result;
// }

#include "../include/pandac.h"

int main() {
    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
    HyperCube<int> hyperCube;

    hyperCube.fit(inputData);
    cout << "Classifier is fit" << endl;
    // hyperCube.transform();
    // cout << "Classifier is transformed" << endl;

    delete inputData;
    return 0;
}