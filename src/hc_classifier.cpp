#include <math.h>
#include "../include/hc_classifier.h"

#define HASH_SIZE (log2(data->getRows())-1)

using namespace std;

template <typename NumCDataType>
HyperCube<NumCDataType>::~HyperCube() {
    if (hashTable != NULL) {
        delete hashTable;
        hashTable = NULL;
    }
    data = NULL;
    hashTableSize = 0;
    k = 0;
}

template <typename NumCDataType>
void HyperCube<NumCDataType>::fit(NumC<NumCDataType>* _data) {
    data = _data;
    hashTableSize = 0;
    hashTable = new HashTable<NumCDataType>(HC, 1<<(int)HASH_SIZE, HASH_SIZE, data->getCols(), 10);
    hashTable->fit(data);
}

template <typename NumCDataType>
void HyperCube<NumCDataType>::transform() {
    hashTable->fit(data);
}

template <typename NumCDataType>
void HyperCube<NumCDataType>::fit_transform(NumC<NumCDataType>* _data) {
    fit(_data);
    transform();
}

template <typename NumCDataType>
Results* HyperCube<NumCDataType>::predict_knn(Vector<NumCDataType> vector, int k, int maxPoints, int maxVertices) {
    int verticesProbed = 0;
    int pointesChecked = 0;
    // comparator to get best results distances
    ResultsComparator resultsComparator(k);
    std::vector<Node<NumCDataType>> bucket;
    // search and find the k with minimun distance
    clock_t start = clock();
    // Get first bucket index (vector's bucket)
    int i = 0;
    unsigned int hashValue = hashTable.hash(vector);
    while (verticesProbed < maxVertices && pointesChecked < maxPoints) {
        bucket = hashTable.getBucket(hashValue);
        for (int j=0; j < bucket.size(); j++) {
            // add to results and the will figure out the best neighbors
            resultsComparator.addResult(j, NumC<NumCDataType>::dist(this->data->getVector(j), vector, 1));
            // resultsComparator.addResult(row, NumC<NumCDataType>::distSparse(this->data->getVector(row), vector, 1));
            if (++pointesChecked == maxPoints) break;
        }
        // Get next bucket index
        hashValue = get_nearestHash(vector, ++i);
        verticesProbed++;
    }
    clock_t end = clock();

    // results 
    Results* results = resultsComparator.getResults();
    results->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);
    return results;
}

template <typename NumCDataType>
Results* HyperCube<NumCDataType>::predict_knn(NumC<NumCDataType>* testData) {
    int numOfQueries = testData->getRows();
    // allocate results sruct for given k
    Results* totalResults = new Results(numOfQueries, this->numOfNeighbors); 
    Results* queryResults;

    // search every row data entry and find the k with minimun distance
    clock_t start = clock();
    for (int query = 0; query < numOfQueries; query++){
        // add to results the results of every query
        queryResults = this->predict_knn(testData->getVector(query));
        totalResults->resultsIndexArray.addVector(queryResults->resultsIndexArray.getVector(0), query);
        totalResults->resultsDistArray.addVector(queryResults->resultsDistArray.getVector(0), query);

        // free query results
        delete queryResults;

    }
    clock_t end = clock();

    // results 
    totalResults->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);
    return totalResults;
}

template <typename NumCDataType>
std::vector<int> HyperCube<NumCDataType>::predict_rs(Vector<NumCDataType> vector, int r, int maxPoints, int maxVertices) {
    double dist;
    int verticesProbed = 0;
    int pointesChecked = 0;
    std::vector<int> data_instances;
    std::vector<Node<NumCDataType>> bucket;
    // Get first bucket index (vector's bucket)
    int i=0;
    unsigned int hashValue = hashTable.hash(vector);
    while (verticesProbed < maxVertices && pointesChecked < maxPoints) {
        bucket = hashTable.getBucket(hashValue);
        for (int j=0; j < bucket.size(); j++) {
            dist = NumC<NumCDataType>::dist(vector, bucket[j].sVector, 1);
            if (dist <= r) {
                data_instances.push_back(bucket[j].index);
            }
            if (++pointesChecked == maxPoints) break;
        }
        // Get next bucket index
        hashValue = get_nearestHash(vector, ++i);
        verticesProbed++;
    }

    return data_instances;
}

#include "../include/pandac.h"

int main() {
    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
    HyperCube<int> hyperCube;

    hyperCube.fit(inputData);
    cout << "Classifier is fit" << endl;
    hyperCube.transform();
    cout << "Classifier is transformed" << endl;


    delete inputData;
    return 0;
}