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
    hashTable = new HashTable<NumCDataType>(HC, hashTableSize, HASH_SIZE, data->getCols(), 10000);
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
std::vector<unsigned int> HyperCube<NumCDataType>::getHashList(Vector<NumCDataType> vector, int maxVertices) {

    std::vector<unsigned int> hashList;
    unsigned int hashValue = hashTable->hash(vector);
    cout << hashValue << endl;
    hashList.push_back(hashValue);

    cout << hashList.size() << endl;

    for (int i=0; i<(int)HASH_SIZE; i++) {
        // cout << i << " changes" << endl;
        get_nearestHashes(hashValue, (int)HASH_SIZE-1, i+1, &hashList, maxVertices);
        // cout << hashList.size() << endl;
        if (hashList.size() >= maxVertices)
            break;
    }

    return hashList;
}


template <typename NumCDataType>
Results* HyperCube<NumCDataType>::predict_knn(Vector<NumCDataType> vector, int k, int maxPoints, int maxVertices) {
    if (maxVertices > hashTableSize) maxVertices = hashTableSize;

    // comparator to get best results distances
    ResultsComparator resultsComparator(k);
    std::vector<Node<NumCDataType>> bucket;
    std::vector<unsigned int> hashList = getHashList(vector,  maxVertices); 

    clock_t start = clock();
    int pointsChecked = 0;

    for (int i = 0; i < hashList.size(); i++){
        bucket = hashTable->getBucket(hashList[i]);
        // search and find the k with minimun distance
        for (int j=0; j < bucket.size(); j++) {
            // add to results and the will figure out the best neighbors
            resultsComparator.addResult(bucket[j].index, NumC<NumCDataType>::dist(bucket[j].sVector, vector, 1));
            pointsChecked++;
        }
        if(pointsChecked >= maxPoints)
            break;
    }
    clock_t end = clock();

    // results 
    Results* results = resultsComparator.getResults();
    results->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);
    return results;
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
Results* HyperCube<NumCDataType>::predict_rs(Vector<NumCDataType> vector, int r, int maxPoints, int maxVertices) {
    if (maxVertices > hashTableSize) maxVertices = hashTableSize;
    // comparator to get best results distances
    ResultsComparator resultsComparator(0);
    std::vector<Node<NumCDataType>> bucket;
    std::vector<unsigned int> hashList = getHashList(vector,  maxVertices); 

    clock_t start = clock();
    int pointsChecked = 0;
    double dist;

    for (int i = 0; i < hashList.size(); i++){
        bucket = hashTable->getBucket(hashList[i]);
        // search and find the k with minimun distance
        for (int j=0; j < bucket.size(); j++) {
            // add to results and the will figure out the best neighbors
            dist = NumC<NumCDataType>::dist(bucket[j].sVector, vector, 1);
            if (dist <= r){
                resultsComparator.addResult(bucket[j].index, dist);
            }
            pointsChecked++;
            // cout << "ckeced " << pointsChecked  << " size " << bucket.size() << " i "<< i<<endl; 
        }
        if(pointsChecked >= maxPoints)
            break;
    }
    clock_t end = clock();
    // for (int i = 0; i < hashTable->getNumOfBuckets(); i+=1){
    //     cout << hashTable->getBucket(i).size() << " ";
    //     if (i > 0 && i%20 == 0) cout << endl;
    // }

    // results 
    Results* results = resultsComparator.getResults();
    results->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);
    return results;
}

// g++ -g ./src/hashtable.cpp ./src/pandac.cpp ./src/numc.cpp ./src/hash_function.cpp ./src/hc_classifier.cpp ./src/prediction_results.cpp
#include "../include/pandac.h"
int main() {
    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
    NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");
    HyperCube<int> hyperCube;

    cout << "HyperCube fit" << endl;
    hyperCube.fit(inputData);
    cout << "HyperCube transform" << endl;
    hyperCube.transform();

    NumC<int>* inputData_ = new NumC<int>(10, inputData->getCols(), true);
    for (int i = 0; i < 10; i++){
        inputData_->addVector(inputData->getVector(i), i);
    }

    cout << "Classifier knn predict" << endl;
    Results* results;
    // results = hyperCube.predict_knn(inputData_, 20, 20000, 20);
    results = hyperCube.predict_rs(inputData_->getVector(0), 25000, 500, 20);
    
    ResultsComparator::print(results, inputDatalabels);
    
    // cout << "Classifier range search" << endl;
    // hyperCube.predict_rs(inputData_, 10, 10, 2);

    delete results;
    delete inputData;
    delete inputData_;
    // delete inputDatalabels;

    return 0;
}