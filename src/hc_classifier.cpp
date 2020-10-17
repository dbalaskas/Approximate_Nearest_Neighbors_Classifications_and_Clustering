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
    hashTableSize = 1<<(int)HASH_SIZE;
    hashTable = new HashTable<NumCDataType>(HC, hashTableSize, HASH_SIZE, data->getCols(), 10);
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
int go = 0;
template <typename NumCDataType>
void HyperCube<NumCDataType>::get_nearestHashes(unsigned int hashValue, int k, int changesLeft, std::vector<unsigned int>* hashList, int maxVertices) {
    if (hashList->size() == maxVertices) return;
    // cout << changesLeft << " ";
    if (changesLeft == 0) {
        hashList->push_back(hashValue);
        go++;
        return;
    }
    if (k<0) return;
    unsigned int mask = 1 << (k);
    // cout << "hashValue: " << hashValue << endl;
    // cout << "hashValue with mask: " << (hashValue^mask) << endl;
    hashValue = hashValue^mask;
    get_nearestHashes(hashValue, k-1, changesLeft-1, hashList, maxVertices);
    hashValue = hashValue^mask;
    get_nearestHashes(hashValue, k-1, changesLeft, hashList, maxVertices);
}

template <typename NumCDataType>
Results* HyperCube<NumCDataType>::predict_knn(Vector<NumCDataType> vector, int k, int maxPoints, int maxVertices) {
    int verticesProbed = 0;
    int pointesChecked = 0;
    if (maxVertices > hashTableSize) maxVertices = hashTableSize;
    // comparator to get best results distances
    ResultsComparator resultsComparator(k);
    std::vector<Node<NumCDataType>> bucket;
    // search and find the k with minimun distance
    clock_t start = clock();
    // Get first bucket index (vector's bucket)
    std::vector<unsigned int> hashList;
    unsigned int hashValue = hashTable->hash(vector);
    cout << hashValue << endl;
    hashList.push_back(hashValue);

    cout << hashList.size() << endl;

    for (int i=0; i<(int)HASH_SIZE; i++) {
        cout << i << " changes" << endl;
        get_nearestHashes(hashValue, (int)HASH_SIZE-1, i+1, &hashList, maxVertices);
        cout << hashList.size() << endl;
        if (hashList.size() >= maxVertices)
            break;
    }
    int bucketNum = 0;
    for (int i = 0; i < hashList.size(); i++){
        cout << ">" << hashList[i] << endl;
    }
    
    // while (bucketNum < maxVertices && bucketNum < maxPoints) {
    //     cout << "before getBucket " << hashList[bucketNum] << endl;
    //     bucket = hashTable->getBucket(hashList[bucketNum++]);
    //     cout << "after getBucket"  <<endl;
        
        // for (int j=0; j < bucket.size(); j++) {
        //     // add to results and the will figure out the best neighbors
        //     resultsComparator.addResult(bucket[j].index, NumC<NumCDataType>::dist(bucket[j].sVector, vector, 1));
        //     // resultsComparator.addResult(row, NumC<NumCDataType>::distSparse(bucket[j].sVector, vector, 1));
        //     // if (++pointesChecked == maxPoints) break;
        // }
        // if (++verticesProbed < maxVertices)
            // hashValue = hashTable->get_nearestHash(vector, verticesProbed);
        // Get next bucket index
    // }
    clock_t end = clock();

    // results 
    // Results* results = resultsComparator.getResults();
    // results->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);
    return 0;
}

template <typename NumCDataType>
Results* HyperCube<NumCDataType>::predict_knn(NumC<NumCDataType>* testData, int k, int maxPoints, int maxVertices) {
    int numOfQueries = testData->getRows();
    // allocate results sruct for given k
    Results* totalResults = new Results(numOfQueries, k); 
    Results* queryResults;

    // search every row data entry and find the k with minimun distance
    clock_t start = clock();
    for (int query = 0; query < numOfQueries; query++){
        // add to results the results of every query
        queryResults = this->predict_knn(testData->getVector(query), k, maxPoints, maxVertices);
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
    unsigned int hashValue = hashTable->hash(vector);
    while (verticesProbed < maxVertices && pointesChecked < maxPoints) {
        bucket = hashTable->getBucket(hashValue);
        for (int j=0; j < bucket.size(); j++) {
            dist = NumC<NumCDataType>::dist(bucket[j].sVector, vector, 1);
            if (dist <= r) {
                data_instances.push_back(bucket[j].index);
            }
            if (++pointesChecked == maxPoints) break;
        }
        // // Get next bucket index
        // hashValue = hashTable->get_nearestHash(vector, ++i);
        // verticesProbed++;
    }

    return data_instances;
}

// g++ -g ./src/hashtable.cpp ./src/pandac.cpp ./src/numc.cpp ./src/hash_function.cpp ./src/hc_classifier.cpp ./src/prediction_results.cpp
#include "../include/pandac.h"
int main() {
    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
    // NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");
    HyperCube<int> hyperCube;

    cout << "HyperCube fit" << endl;
    hyperCube.fit(inputData);
    cout << "HyperCube transform" << endl;
    // hyperCube.transform();

    NumC<int>* inputData_ = new NumC<int>(10, inputData->getCols(), true);
    for (int i = 0; i < 10; i++){
        inputData_->addVector(inputData->getVector(i), i);
    }

    cout << "Classifier knn predict" << endl;
    Results* results;
    results = hyperCube.predict_knn(inputData_->getVector(0), 10, 20000, 200);

    // results.print();
    // results->resultsIndexArray.print();
    // ResultsComparator::print(results, inputDatalabels);
    
    // cout << "Classifier range search" << endl;
    // hyperCube.predict_rs(inputData_, 10, 10, 2);

    delete results;
    delete inputData;
    delete inputData_;
    // delete inputDatalabels;

    return 0;
}