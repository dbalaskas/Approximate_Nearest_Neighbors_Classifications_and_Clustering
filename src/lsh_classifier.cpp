#include "../include/lsh_classifier.h"

#define HASHTABLE_SIZE data->getRows()/8

using namespace std;

template <typename NumCDataType>
LSHashing<NumCDataType>::LSHashing(int L, int k, int w)
: L{L}, k{k}, w{w}, data{NULL}
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
    ResultsComparator resultsComparator(N);
    unsigned int hashValue;
    std::vector< Node<NumCDataType> > bucket;
    double dist;

    clock_t start = clock();
    for (int i=0; i < L; i++) {
        // get bucket
        hashValue = hashTableList[i]->hash(vector);
        bucket = hashTableList[i]->getBucket(vector);
        for (int j=0; j < (int) bucket.size(); j++) {
            if (hashValue == bucket[j].hashValue) {
                dist = NumC<NumCDataType>::distSparse(vector, bucket[j].sVector, 1);
                resultsComparator.addResult(bucket[j].index, dist);
            }
        }
        // results.add(index, dist);
    }
    clock_t end = clock();

    Results* results = resultsComparator.getResults();
    // results 
    results->executionTime = ((double) (end - start) / (CLOCKS_PER_SEC/1000));

    return results;
}

template <typename NumCDataType>
Results* LSHashing<NumCDataType>::predict_knn(NumC<NumCDataType>* testData, int N) {
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
        totalResults->executionTimeArray.addElement(queryResults->executionTime, query, 0);
        
        // free query results
        delete queryResults;

    }
    clock_t end = clock();
    
    // results 
    totalResults->executionTime = ((double) (end - start) / (CLOCKS_PER_SEC/1000));

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

    // // result.setTime((double) (end - start) / (CLOCKS_PER_SEC/1000));
    // // result.time = (double)(end - start) / (CLOCKS_PER_SEC/1000);
    // return result;

    ResultsComparator resultsComparator(0);
    // ResultIndex result;
    // std::vector<ResultIndex> results;
    unsigned int hashValue;
    std::vector< Node<NumCDataType> > bucket;
    double dist;

    clock_t start = clock();
    for (int i=0; i < L; i++) {
        // get bucket
        hashValue = hashTableList[i]->hash(vector);
        bucket = hashTableList[i]->getBucket(vector);
        for (int j=0; j < (int) bucket.size(); j++) {
            if (hashValue == bucket[j].hashValue) {
                dist = NumC<NumCDataType>::dist(vector, bucket[j].sVector, 1);
                if (dist <= r){
                    resultsComparator.addResult(bucket[j].index, dist);
                }
            }
        }
        // results.add(index, dist);
    }
    clock_t end = clock();

    Results* results = resultsComparator.getResults();
    // results 
    results->executionTime = ((double) (end - start) / (CLOCKS_PER_SEC/1000));

    return results;
}

template <typename NumCDataType>
vector<Results*> LSHashing<NumCDataType>::predict_rs(NumC<NumCDataType>* testData, int r) {
    int numOfQueries = testData->getRows();
    // allocate results sruct for given k
    vector<Results*> totalResults(numOfQueries); 
    Results* queryResults;

    // search every row data entry and find the k with minimun distance
    for (int query = 0; query < numOfQueries; query++){
        // add to results the results of every query
        queryResults = this->predict_rs(testData->getVector(query), r);
        totalResults[query] = queryResults;
    }

    // results
    return totalResults;
}


template <typename NumCDataType>
Results* LSHashing<NumCDataType>::reverse_assignment(NumC<NumCDataType>* centroids) {

    unsigned int hashValue;
    std::vector< Node<NumCDataType> > bucket;

    RA_ResultsComparator resultsComparator(this->data->getRows());
    NumCDistType dist;
    int prev_pointsChecked = resultsComparator.getResultsSize();
    int new_pointsChecked = 0;

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

    // for (int i=0; i < L; i++) {
    //     // get bucket
    //     hashValue = hashTableList[i]->hash(vector);
    //     bucket = hashTableList[i]->getBucket(hashValue%HASHTABLE_SIZE);

    //     for (int j=0; j < (int) bucket.size(); j++) {
    //         if (hashValue == bucket[j].hashValue) {

    //             dist = NumC<NumCDataType>::dist(vector, bucket[j].sVector, 1);
    //             if (dist <= r){
    //                 resultsComparator.addResult(bucket[j].index, dist);
    //             }

    //         }
    //     }
    //     // results.add(index, dist);
    // }
    do{
        prev_pointsChecked = resultsComparator.getResultsSize();

        for (int centroidIndex=0; centroidIndex < centroids->getRows(); centroidIndex++) {
            //return points in distance r
            for (int i=0; i < L; i++) {

                hashValue = hashTableList[i]->hash(centroids->getVector(centroidIndex));
                bucket = hashTableList[i]->getBucket(centroids->getVector(centroidIndex));

                for (int j=0; j < (int) bucket.size(); j++) {
                // cout << "bucket size " <<bucket.size() <<" " <<bucket[j].hashValue << " "<< hashValue<<endl;
                    if (hashValue == bucket[j].hashValue) {
                        
                        if (resultsComparator.checkIndex(bucket[j].index) && resultsComparator.getResult(bucket[j].index).first_cluster != centroidIndex) {

                            dist = NumC<NumCDataType>::dist(bucket[j].sVector, centroids->getVector(centroidIndex), 1);
                            if (dist <= r){
                                resultsComparator.addResultConflict(bucket[j].index, centroidIndex, dist);
                            }
                        } else {
                            dist = NumC<NumCDataType>::dist(bucket[j].sVector, centroids->getVector(centroidIndex), 1);
                            if (dist <= r){
                                resultsComparator.addResult(bucket[j].index, centroidIndex, dist);
                            }
                        }

                    }
                }
            }
        }
        r *= 2;
        new_pointsChecked = resultsComparator.getResultsSize();
        // cout << "NEW POINTS CHECKED [" << new_pointsChecked << "]" <<endl;
    }while (new_pointsChecked != prev_pointsChecked || new_pointsChecked == 0);



    // results 
    Results* results = resultsComparator.getResults();
    // results->resultsIndexArray.print();
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

        // // find the seconde nearest neighbour to fix results for the silouette
        // if (results->resultsIndexArray.getElement(resultIndex,1) == -1){
        //     knnResults = knnEstimator->predict_knn(this->data->getVector(resultIndex));
        //     // add its results (centroids) to the total results
        //     centroidIndex = knnResults->resultsIndexArray.getElement(0,0);
        //     dist = knnResults->resultsDistArray.getElement(0,0);
        //     if ( centroidIndex != results->resultsIndexArray.getElement(resultIndex,0) )
        //         resultsComparator.addResultSecond(resultIndex, centroidIndex, dist);
        //     // add second nearest centroid
        //     centroidIndex = knnResults->resultsIndexArray.getElement(0,1);
        //     dist = knnResults->resultsDistArray.getElement(0,1);
        //     if ( centroidIndex != results->resultsIndexArray.getElement(resultIndex,0) )
        //         resultsComparator.addResultSecond(resultIndex, centroidIndex, dist);
        //     delete knnResults;
        // }
    }
    // delete previous results and gat the new one
    delete results;
    results = resultsComparator.getResults();

    // set time to results
    results->executionTime = ((double) (clock() - start) / (CLOCKS_PER_SEC/1000));

    // results->resultsIndexArray.print();
    delete knnEstimator;
    return results;
}

// #include "../include/pandac.h"
// int main(){
//     NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
//     NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");

//     LSHashing<int> lsh(1,5,4,20000);
//     lsh.fit(inputData);
//     lsh.transform();


//     // NumC<int>* inputData_ = new NumC<int>(100, inputData->getCols(), true);
//     // for (int i = 0; i < 100; i++){
//     //     inputData_->addVector(inputData->getVector(i), i);
//     // }
//     Results* results;
//     // results = lsh.predict_knn(inputData->getVector(9), 50);
//     // delete results;
//     // results = lsh.predict_knn(inputData_, 50);
//     // ResultsComparator::print(results, inputDatalabels);
//     // delete results;
//     // delete inputData_;

    // results = lsh.predict_knn(inputData_, 50);

//     results = lsh.predict_rs(inputData->getVector(0), 40000.0);
//     ResultsComparator::print(results, inputDatalabels);

    


//     delete inputData;
//     delete inputDatalabels;
// }