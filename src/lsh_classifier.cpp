#include "../include/lsh_classifier.h"


#define HASHTABLE_SIZE data->getRows()/8

using namespace std;

template <typename NumCDataType>
LSHashing<NumCDataType>::LSHashing(int N, int L, int k, int w)
: data{NULL}, N{N}, L{L}, k{k}, w{w}
{
    this->hashTableList = new HashTable<NumCDataType>*[this->L];
}


template <typename NumCDataType>
LSHashing<NumCDataType>::~LSHashing() {
    
    for (int i=0; i < L; i++) {
        if (hashTableList[i] != NULL) {
            delete hashTableList[i];
            hashTableList[i] = NULL;
        }
    }
    delete[] hashTableList;
    data = NULL;
    L = 0;
    N = 0;
    k = 0;
    w = 0;
}

template <typename NumCDataType>
void LSHashing<NumCDataType>::fit(NumC<NumCDataType>* _data) {
    this->data = _data;

    for (int i=0; i < L; i++) {
        hashTableList[i] = new HashTable<NumCDataType>(LSH, HASHTABLE_SIZE, this->k, this->data->getCols(), this->w) ;
    }

}

template <typename NumCDataType>
void LSHashing<NumCDataType>::transform() {
    for (int i=0; i < L; i++) {
       hashTableList[i]->fit(data); 
    }
}

template <typename NumCDataType>
void LSHashing<NumCDataType>::fit_transform(NumC<NumCDataType>* _data) {
    fit(_data);
    transform();
}

template <typename NumCDataType>
Results* LSHashing<NumCDataType>::predict_knn(Vector<NumCDataType> vector, int N) {
    if (N == 0)
        N = this->N;

    ResultsComparator resultsComparator(N);
    std::vector< Node<NumCDataType> > bucket;
    double dist;

    clock_t start = clock();
    for (int i=0; i < L; i++) {
        // get bucket
        bucket = hashTableList[i]->getBucket(vector);

        for (int j=0; j < bucket.size(); j++) {
            dist = NumC<NumCDataType>::dist(vector, bucket[j].sVector, 1);
            resultsComparator.addResult(bucket[j].index, dist);
        }
        // results.add(index, dist);
    }
    clock_t end = clock();

    Results* results = resultsComparator.getResults();
    // results 
    results->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);

    return results;
}

template <typename NumCDataType>
Results* LSHashing<NumCDataType>::predict_knn(NumC<NumCDataType>* testData, int N) {
    if (N == 0)
        N = this->N;
    
    int numOfQueries = testData->getRows();
    // allocate results sruct for given k
    Results* totalResults = new Results(numOfQueries, N); 
    Results* queryResults;

    // search every row data entry and find the k with minimun distance
    clock_t start = clock();
    for (int query = 0; query < numOfQueries; query++){

        // add to results the results of every query
        queryResults = this->predict_knn(testData->getVector(query), N);
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
Results* LSHashing<NumCDataType>::predict_rs(Vector<NumCDataType> vector, double r) {
    // Results result = Results();
    // double dist;
    
    // clock_t start = clock();
    // for (int i=0; i < L; i++) {
    //     Bucket bucket = hashTableList[i].getBucket(vector);
    //     for (int j=0; j < bucket.size(); j++) {
    //         dist = NumC<NumCDataType>::dist(vector, bucket[j], 1);
    //         if (dist <= r) {
    //             // results.add(index, dist);
    //         }
    //     }
    // }
    // clock_t end = clock();

    // // result.setTime((double) (end - start) / CLOCKS_PER_SEC);
    // // result.time = (double)(end - start) / CLOCKS_PER_SEC;
    // return result;

    ResultsComparator resultsComparator(0);
    // ResultIndex result;
    // std::vector<ResultIndex> results;
    std::vector< Node<NumCDataType> > bucket;
    double dist;

    clock_t start = clock();
    for (int i=0; i < L; i++) {
        // get bucket
        bucket = hashTableList[i]->getBucket(vector);

        for (int j=0; j < bucket.size(); j++) {
            dist = NumC<NumCDataType>::dist(vector, bucket[j].sVector, 1);
            if (dist <= r){
                resultsComparator.addResult(bucket[j].index, dist);
            }
        }
        // results.add(index, dist);
    }
    clock_t end = clock();

    Results* results = resultsComparator.getResults();
    // results 
    results->executionTime = ((double) (end - start) / CLOCKS_PER_SEC);

    return results;
}

#include "../include/pandac.h"
int main(){
    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
    NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");

    LSHashing<int> lsh(1,5,4,20000);
    lsh.fit(inputData);
    lsh.transform();


    // NumC<int>* inputData_ = new NumC<int>(100, inputData->getCols(), true);
    // for (int i = 0; i < 100; i++){
    //     inputData_->addVector(inputData->getVector(i), i);
    // }
    Results* results;
    // results = lsh.predict_knn(inputData->getVector(9), 50);
    // delete results;
    // results = lsh.predict_knn(inputData_, 50);
    // ResultsComparator::print(results, inputDatalabels);
    // delete results;
    // delete inputData_;


    results = lsh.predict_rs(inputData->getVector(0), 40000.0);
    ResultsComparator::print(results, inputDatalabels);

    


    delete inputData;
    delete inputDatalabels;
}