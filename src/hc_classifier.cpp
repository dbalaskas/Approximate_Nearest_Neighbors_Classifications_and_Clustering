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
    w = 0;
}

template <typename NumCDataType>
void HyperCube<NumCDataType>::fit(NumC<NumCDataType>* _data, int k) {
    data = _data;
    if (k==-1) k = (int)HASH_SIZE;
    hashTableSize = 1<< k;
    hashTable = new HashTable<NumCDataType>(HC, hashTableSize, k, data->getCols(), w);
}

template <typename NumCDataType>
void HyperCube<NumCDataType>::transform() {
    hashTable->fit(data);
}

template <typename NumCDataType>
void HyperCube<NumCDataType>::fit_transform(NumC<NumCDataType>* _data, int k) {
    fit(_data, k);
    transform();
}
int go = 0;
template <typename NumCDataType>
void HyperCube<NumCDataType>::get_nearestHashes(unsigned int hashValue, int k, int changesLeft, std::vector<unsigned int>* hashList, int maxVertices) {
    if ((int) hashList->size() == maxVertices) return;
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
    // cout << hashValue << endl;
    hashList.push_back(hashValue);

    // cout << hashList.size() << endl;

    for (int i=0; i<(int)HASH_SIZE; i++) {
        // cout << i << " changes" << endl;
        get_nearestHashes(hashValue, (int)HASH_SIZE-1, i+1, &hashList, maxVertices);
        // cout << hashList.size() << endl;
        if ((int) hashList.size() >= maxVertices)
            break;
    }

    return hashList;
}


template <typename NumCDataType>
Results* HyperCube<NumCDataType>::predict_knn(Vector<NumCDataType> vector, int k, int maxPoints, int maxVertices) {
    if (maxVertices > hashTableSize) maxVertices = hashTableSize;
    if (k > maxPoints) k = maxPoints;
    // comparator to get best results distances
    ResultsComparator resultsComparator(k);
    std::vector<Node<NumCDataType>> bucket;
    std::vector<unsigned int> hashList = getHashList(vector,  maxVertices); 

    clock_t start = clock();
    int pointsChecked = 0;

    for (int i = 0; i < maxVertices; i++){
        bucket = hashTable->getBucket(hashList[i]);
        // search and find the k with minimun distance
        for (int j=0; j < (int) bucket.size(); j++) {
            // add to results and the will figure out the best neighbors
            resultsComparator.addResult(bucket[j].index, NumC<NumCDataType>::distSparse(bucket[j].sVector, vector, 1));
            pointsChecked++;
            if(pointsChecked >= maxPoints) {
                break;
            }
        }
        if(pointsChecked >= maxPoints) {
            break;
        }
    }
    clock_t end = clock();

    // results 
    Results* results = resultsComparator.getResults();
    results->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);
    return results;
}

template <typename NumCDataType>
Results* HyperCube<NumCDataType>::predict_knn(NumC<NumCDataType>* testData, int k, int maxPoints, int maxVertices) {
    if (maxVertices > hashTableSize) maxVertices = hashTableSize;
    if (k > maxPoints) k = maxPoints;
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
        totalResults->executionTimeArray.addElement(queryResults->executionTime, query, 0);
        
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

    for (int i = 0; i < maxVertices; i++){
        bucket = hashTable->getBucket(hashList[i]);
        // search and find the k with minimun distance
        for (int j=0; j < (int) bucket.size(); j++) {
            // add to results and the will figure out the best neighbors
            dist = NumC<NumCDataType>::dist(bucket[j].sVector, vector, 1);
            if (dist <= r){
                resultsComparator.addResult(bucket[j].index, dist);
            }
            pointsChecked++;
            if(pointsChecked >= maxPoints) {
                break;
            }
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

template <typename NumCDataType>
vector<Results*> HyperCube<NumCDataType>::predict_rs(NumC<NumCDataType>* testData, int r, int maxPoints, int maxVertices) {
    if (maxVertices > hashTableSize) maxVertices = hashTableSize;
    int numOfQueries = testData->getRows();
    // allocate results sruct for given k
    vector<Results*> totalResults(numOfQueries); 
    Results* queryResults;

    // search every row data entry and find the k with minimun distance
    for (int query = 0; query < numOfQueries; query++){
        // add to results the results of every query
        queryResults = this->predict_rs(testData->getVector(query), r, maxPoints, maxVertices);
        totalResults[query] = queryResults;
    }

    // results
    return totalResults;
}

template <typename NumCDataType>
Results* HyperCube<NumCDataType>::reverse_assignment(NumC<NumCDataType>* centroids, int maxPoints, int maxVertices) {
    if (maxVertices > hashTableSize) maxVertices = hashTableSize;
    // // comparator to get best results distances
    // ResultsComparator resultsComparator(0);
    // std::vector<Node<NumCDataType>> bucket;
    // std::vector<unsigned int> hashList = getHashList(vector,  maxVertices); 

    // int pointsChecked = 0;
    // double dist;

    // for (int i = 0; i < (int) hashList.size(); i++){
    //     bucket = hashTable->getBucket(hashList[i]);
    //     // search and find the k with minimun distance
    //     for (int j=0; j < (int) bucket.size(); j++) {
    //         // add to results and the will figure out the best neighbors
    //         dist = NumC<NumCDataType>::dist(bucket[j].sVector, vector, 1);
    //         if (dist <= r){
    //             resultsComparator.addResult(bucket[j].index, dist);
    //         }
    //         pointsChecked++;
    //         // cout << "ckeced " << pointsChecked  << " size " << bucket.size() << " i "<< i<<endl; 
    //     }
    //     if(pointsChecked >= maxPoints)
    //         break;
    // }
    // clock_t end = clock();
    // // for (int i = 0; i < hashTable->getNumOfBuckets(); i+=1){
    // //     cout << hashTable->getBucket(i).size() << " ";
    // //     if (i > 0 && i%20 == 0) cout << endl;
    // // }
    RA_ResultsComparator resultsComparator(this->data->getRows());

    NumCDistType dist;
    // int clusteredPoints = 0;
    int prev_pointsChecked = resultsComparator.getResultsSize();
    int new_pointsChecked = 0;
    // Results* queryResults;
    // ResultsComparator resultsComparator(0);
    std::vector<Node<NumCDataType>> bucket;
    std::vector<unsigned int> hashList;

    //* GOOD MORNING BUDDY
    // r-Computation
    //  <3 <3 <3 <3 
    clock_t start = clock();
    NumCDistType r = NumC<NumCDataType>::dist(centroids->getVector(0), centroids->getVector(1), 1);
    for (int i = 0; i < centroids->getRows(); i++){
        for (int j=i+1; j < centroids->getRows(); j++) {
            dist = NumC<NumCDataType>::dist(centroids->getVector(i), centroids->getVector(j), 1);
            if (dist < r) {
                r = dist;
            }
        }
    }
    r /= 2;
    cout << r << endl;
//     //* ^^^^^^^^^ NOT TESTED ^^^^^^^^^
    // while (clusteredPoints < data->getRows()) {
    do{
        prev_pointsChecked = resultsComparator.getResultsSize();

        for (int centroidIndex=0; centroidIndex < centroids->getRows(); centroidIndex++) {
            //return points in distance r
            // queryResults = this->predict_rs(centroids->getVector(cetroidIndex), r, maxPoints, maxVertices);
            hashList = getHashList(centroids->getVector(centroidIndex),  maxVertices);
            // for (size_t i = 0; i < hashList.size(); i++){
            //     cout<< "list " <<hashList[i]<<endl;
            // }
            
            // pointsChecked = 0;
            for (int i = 0; i < maxVertices; i++){
                bucket = hashTable->getBucket(hashList[i]);
                // cout << "b size "<<bucket.size()<<endl;
                // search and find the k with minimun distance
                for (int j=0; j < (int) bucket.size(); j++) {
                    // add to results and the will figure out the best neighbors
                    // cout << "Index: [" << bucket[j].index << "]" << endl;
                    if (resultsComparator.checkIndex(bucket[j].index) && resultsComparator.getResult(bucket[j].index).first_cluster != centroidIndex) {
                        // // check if confliict
                        // if (map[bucket[j].index] != cetroidIndex) {
                        //     if (dist < =map[bucket[j].index].distance) {
                        //         //custom insert in map
                        //         clusteredPoints++;
                        //     }
                        // } // else continue
                        dist = NumC<NumCDataType>::dist(bucket[j].sVector, centroids->getVector(centroidIndex), 1);
                        // cout << "Conflict centroid "<< centroidIndex << " vector " << bucket[j].index <<endl; 
                        if (dist <= r){
                            resultsComparator.addResultConflict(bucket[j].index, centroidIndex, dist);
                        }
                    } else {
                        dist = NumC<NumCDataType>::dist(bucket[j].sVector, centroids->getVector(centroidIndex), 1);
                        if (dist <= r){
                            resultsComparator.addResult(bucket[j].index, centroidIndex, dist);

                            //custom insert in map
                            // clusteredPoints++;
                        }
                    }
                    // pointsChecked++;
                    // cout << "ckeced " << pointsChecked  << " size " << bucket.size() << " i "<< i<<endl; 
                }
                if(resultsComparator.getResultsSize() >= maxPoints)
                    break;
            }
        }
        r *= 2;
        new_pointsChecked = resultsComparator.getResultsSize();
        cout << "NEW POINTS CHECKED [" << new_pointsChecked << "]" <<endl;
    }while (new_pointsChecked != prev_pointsChecked || new_pointsChecked == 0);


    // results 
    Results* results = resultsComparator.getResults();
    // results->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);
    // results->resultsDistArray.print();
    // do exhaustive search for the points that remained unassigned
    Results* knnResults;
    NumCIndexType centroidIndex;
    ExhaustiveKnn<NumCDataType>* knnEstimator = new ExhaustiveKnn<NumCDataType>(2);
    knnEstimator->fit(centroids);
    
    // find the unassigned with index -1
    for (int resultIndex = 0; resultIndex < results->resultsIndexArray.getRows(); resultIndex++){
        
        // if found unassigned then do exhaustive search for it
        if (results->resultsIndexArray.getElement(resultIndex,0) == -1){
            knnResults = knnEstimator->predict_knn(this->data->getVector(resultIndex));
            // add its results (centroids) to the total results
            centroidIndex = knnResults->resultsIndexArray.getElement(0,0);
            dist = knnResults->resultsDistArray.getElement(0,0);
            resultsComparator.addResult(resultIndex, centroidIndex, dist);
            // add second nearest centroid
            centroidIndex = knnResults->resultsIndexArray.getElement(0,1);
            dist = knnResults->resultsDistArray.getElement(0,1);
            resultsComparator.addResultConflict(resultIndex, centroidIndex, dist);
            delete knnResults;
        }
    }
    // delete previous results and gat the new one
    delete results;
    results = resultsComparator.getResults();

    // set time to results
    results->executionTime = ((double) (clock() - start) / CLOCKS_PER_SEC);

    delete knnEstimator;
    return results;
}

// // g++ -g ./src/hashtable.cpp ./src/pandac.cpp ./src/numc.cpp ./src/hash_function.cpp ./src/hc_classifier.cpp ./src/prediction_results.cpp
// #include "../include/pandac.h"
// int main() {
//     NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
//     NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");
//     HyperCube<int> hyperCube;

//     cout << "HyperCube fit" << endl;
//     hyperCube.fit(inputData);
//     cout << "HyperCube transform" << endl;
//     hyperCube.transform();

//     NumC<int>* inputData_ = new NumC<int>(10, inputData->getCols(), true);
//     for (int i = 0; i < 10; i++){
//         inputData_->addVector(inputData->getVector(i), i);
//     }

//     cout << "Classifier knn predict" << endl;
//     Results* results;
//     // results = hyperCube.predict_knn(inputData_, 20, 20000, 20);
//     results = hyperCube.predict_rs(inputData_->getVector(0), 25000, 500, 20);
    
//     ResultsComparator::print(results, inputDatalabels);
    
//     // cout << "Classifier range search" << endl;
//     // hyperCube.predict_rs(inputData_, 10, 10, 2);

//     delete results;
//     delete inputData;
//     delete inputData_;
//     // delete inputDatalabels;

//     return 0;
// }