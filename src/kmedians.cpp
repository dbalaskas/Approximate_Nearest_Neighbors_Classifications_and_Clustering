#include <numeric>
#include <random>
#include <queue>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "../include/kmedians.h"
#include "../include/exhaustive_knn.h"
#include "../include/hc_classifier.h"
#include "../include/lsh_classifier.h"

using namespace std;

ConfigurationData readConfiguration(char* configurationFile) {
    ConfigurationData confData;
    FILE *conf = fopen(configurationFile, "r");
    
    char line[128];
    char *command;
    int value;
    while(fgets(line,sizeof(line),conf) != NULL) {
        if (line[0] == '#') {
            // Comment
            continue;
        }
        if (strlen(line) > 1) {
            // cout << line;
    		command = strtok(line," : ");
    		value = atoi(strtok(NULL,"\n"));
        }
        // cout << "<" << command << ">: " << value << endl;
        if (!strcmp(command, (char*) "number_of_clusters")) {
            confData.number_of_clusters = value;
        } else if (!strcmp(command, (char*) "number_of_vector_hash_tables")) {
            confData.L = value;
        } else if (!strcmp(command, (char*) "number_of_vector_hash_functions")) {
            confData.k = value;
        } else if (!strcmp(command, (char*) "max_number_M_hypercube")) {
            confData.M = value;
        } else if (!strcmp(command, (char*) "number_of_hypercube_dimensions")) {
            confData.d = value;
        } else if (!strcmp(command, (char*) "number_of_probes")) {
            confData.probes = value;
        } else {
            // Not accepted configuration
        }
    }
    if (!feof(conf)) {
      	cout << "\033[0;31mError!\033[0m Bad configuration file." << endl;
        fclose(conf);
        return ConfigurationData();
    }

    fclose(conf);
    return confData;
}


template <typename NumCDataType> 
Kmedians<NumCDataType>::Kmedians(int numOfClusters, int maxIterations, NumCDistType error){
    this->numOfClusters = numOfClusters;
    this->numOfDimensions = 0;
    this->numOfPoints = 0;
    this->maxIterations = maxIterations;
    this->error = error;
    this->transformTime = 0.0;
    // this->overallSilhouette = 0.0;
    this->centroids = NULL;
    this->data = NULL;
    this->lastResults = NULL;
    // this->objectiveCost = 0.0;
}

template <typename NumCDataType> 
Kmedians<NumCDataType>::Kmedians(ConfigurationData configurationData, int maxIterations, NumCDistType error){
    this->numOfClusters = configurationData.number_of_clusters;
    this->numOfDimensions = 0;
    this->numOfPoints = 0;
    this->maxIterations = maxIterations;
    this->error = error;
    this->transformTime = 0.0;

    this->L = configurationData.L;
    this->k = configurationData.k;                  
    this->M = configurationData.M;                  
    this->d = configurationData.d;                  
    this->probes = configurationData.probes;             

    this->data = data;
    this->centroids = NULL;
    this->lastResults = NULL;
    // this->silhouette.reserve(this->numOfClusters);
    // this->overallSilhouette = 0.0;
    // this->objectiveCost = 0.0;
}

template <typename NumCDataType> 
Kmedians<NumCDataType>::Kmedians(NumC<NumCDataType>* data, int numOfClusters, int maxIterations, NumCDistType error){
    this->numOfClusters = numOfClusters;
    this->numOfDimensions = this->data->getCols();
    this->numOfPoints = this->data->getRows();
    this->maxIterations = maxIterations;
    this->error = error;
    this->transformTime = 0.0;

    this->data = data;
    this->centroids = NULL;
    this->lastResults = NULL;
    // this->silhouette.reserve(this->numOfClusters);
    // this->overallSilhouette = 0.0;
    // this->objectiveCost = 0.0;
}

template <typename NumCDataType> 
Kmedians<NumCDataType>::~Kmedians(){
    this->numOfClusters = 0;

    if (this->data != NULL) {
        this->data = NULL;
    }
    if (this->centroids != NULL) {
        delete this->centroids;
        this->centroids = NULL;
    }
    if (this->lastResults != NULL){
        delete this->lastResults;
        this->lastResults = NULL;
    }


}


template <typename NumCDataType> 
void Kmedians<NumCDataType>::fit(NumC<NumCDataType>* trainData){
    this->data = trainData;
    this->numOfDimensions = this->data->getCols();
    this->numOfPoints = this->data->getRows();

}

template <typename NumCDataType> 
void Kmedians<NumCDataType>::randomInit(){
    // allocate random for centroids
    if (this->centroids != NULL) {
        delete this->centroids;
        this->centroids = NULL;
    }
    this->centroids = new NumC<NumCDataType>(this->numOfClusters, this->numOfDimensions);
    NumCDataType max = this->data->max();
    this->centroids->random(max);
}

template <typename NumCDataType> 
void Kmedians<NumCDataType>::kmeansInit(){

    srand(time(NULL));
    std::random_device randomDevice; 
    std::mt19937 generator(randomDevice()); 


    // allocate space for the first centroid
    if (this->centroids != NULL) {
        delete this->centroids;
        this->centroids = NULL;
    }
    this->centroids = new NumC<NumCDataType>(1, this->numOfDimensions);

    ExhaustiveKnn<NumCDataType>* initEstimator = new ExhaustiveKnn<NumCDataType>(1);
    Results* results;
    NumCDistType lastElement;
    NumCDistType randomX;
    NumCIndexType randomXindex;
    // pick a random cantroid to start with
    randomXindex = rand()%this->numOfPoints;
    this->centroids->addVector(this->data->getVector(randomXindex), 0);

    // start kmeans++
    for (int centroidIndex = 1; centroidIndex < this->numOfClusters; centroidIndex++){
        
        // get the min distance of each point to centroids
        initEstimator->fit(this->centroids);
        results = initEstimator->predict_knn(this->data);

        // normalize and accumulate the values
        // square
        results->resultsDistArray.square();
        results->resultsDistArray.normalize();
        results->resultsDistArray.cumulative();

        // get a random number between [0,P(n−t)]
        lastElement = results->resultsDistArray.getLast();
        std::uniform_real_distribution<NumCDistType> distribution(0,lastElement);
        randomX = (NumCDistType)distribution(generator);
        
        // get the index between
        randomXindex = results->resultsDistArray.find(randomX);

        // add to centroids this element of this index
        this->centroids->appendVector(this->data->getVector(randomXindex));
        
        // results->resultsDistArray.print();
        delete results;
    }
     
    delete initEstimator;
}




template <typename NumCDataType> 
NumC<NumCDataType>* Kmedians<NumCDataType>::getCentroids(){
    return this->centroids;
}

// calculate and get the indexes of images that belong to known centroids
template <typename NumCDataType> 
vector<Results*> Kmedians<NumCDataType>::getResults(){

    // allocate space for new results
    vector<Results*> totalResults(this->numOfClusters); 
    ResultsComparator* resultsComparator;

    // ExhaustiveKnn<NumCDataType>* knnEstimator = new ExhaustiveKnn<NumCDataType>(1);
    // knnEstimator->fit(this->centroids);
    // Results* resultsKnn = knnEstimator->predict_knn(this->data);
    Results* resultsKnn = this->lastResults;

    for (int centroidIndex = 0; centroidIndex < this->numOfClusters; centroidIndex++){

        resultsComparator = new ResultsComparator(0);

        for (int resultsIndex = 0; resultsIndex < resultsKnn->resultsIndexArray.getRows(); resultsIndex++){
            if ( resultsKnn->resultsIndexArray.getElement(resultsIndex, 0) ==  centroidIndex){
                resultsComparator->addResult(resultsIndex, resultsKnn->resultsDistArray.getElement(resultsIndex, 0));
            }
        }
        totalResults[centroidIndex] = resultsComparator->getResults();
        totalResults[centroidIndex]->executionTime = this->transformTime;
        delete resultsComparator;
    }

    // delete resultsKnn;
    // delete knnEstimator;
    return totalResults;
}

template <typename NumCDataType> 
NumCDistType Kmedians<NumCDataType>::calculateSilhouette(NumCDistType distA, NumCDistType distB){

    if (distA < distB){
        return (1.0 - (distA / distB));
    }
    else if (distA == distB){
        return 0;
    }
    else if (distA > distB){
        return  ((distB / distA) - 1.0);
    }

}

template <typename NumCDataType> 
vector<NumCDistType> Kmedians<NumCDataType>::getSilhouettes(Results* results){

    vector<NumCDistType> meanDistA;
    // meanDistA.reserve(this->numOfPoints / this->numOfClusters);
    vector<NumCDistType> meanDistB;
    // meanDistB.reserve(this->numOfPoints / this->numOfClusters);
    vector< vector<NumCDistType> > silhouettes(this->numOfClusters);
    vector<NumCDistType> overallSilhouettes(this->numOfClusters);
    NumCDistType meanSilhouettes;
    NumCDistType distA = 0;
    NumCDistType distB = 0;
    NumCDistType meanA = 0;
    NumCDistType meanB = 0;
    NumCIndexType AcentroidIndex;
    NumCIndexType BcentroidIndex;
    // NumCIndexType indexForDist;
    NumCDistType meanA_ = 0;
    NumCDistType meanB_ = 0;
    NumCIndexType sizeA = 0;
    NumCIndexType sizeB = 0;
    NumCDistType sumSi = 0;

    clock_t start = clock();
    for (int point = 0; point < this->numOfPoints; point++){
        AcentroidIndex = results->resultsIndexArray.getElement(point, 0);
        BcentroidIndex = results->resultsIndexArray.getElement(point, 1);

        meanDistA.clear();
        meanDistB.clear();
        meanA_ = 0;
        meanB_ = 0;
        sizeA = 0;
        sizeB = 0;
        for (int resultsIndex = 0; resultsIndex < results->resultsIndexArray.getRows(); resultsIndex++){

            if ( results->resultsIndexArray.getElement(resultsIndex, 0) ==  AcentroidIndex && resultsIndex != point){
                // distA = results->resultsDistArray.getElement(resultsIndex, 0);
                distA = NumC<NumCDataType>::dist(this->data->getVector(resultsIndex), this->data->getVector(point), 1);
                meanDistA.push_back(distA);
                meanA_ += distA;
                sizeA++;
            }
            else if ( results->resultsIndexArray.getElement(resultsIndex, 0) ==  BcentroidIndex && resultsIndex != point){
                // distB = results->resultsDistArray.getElement(resultsIndex, 1);
                distB = NumC<NumCDataType>::dist(this->data->getVector(resultsIndex), this->data->getVector(point), 1);
                meanDistB.push_back(distB);
                meanB_ += distB;
                sizeB++;
            }
        }
        // get the silhouette
        // meanA = std::accumulate(meanDistA.begin(), meanDistA.end(), 0.0) / (NumCDistType)meanDistA.size();
        // meanB = std::accumulate(meanDistB.begin(), meanDistB.end(), 0.0) / (NumCDistType)meanDistB.size();
        meanA = meanA_ / sizeA;
        meanB = meanB_ / sizeB;

        // this->silhouette.push_back(calculateSilhouette( meanA, meanB));
        silhouettes[AcentroidIndex].push_back(calculateSilhouette( meanA, meanB));
        sumSi += calculateSilhouette( meanA, meanB);
        // cout<< calculateSilhouette( meanA, meanB)<<" " << AcentroidIndex << " " << silhouettes[AcentroidIndex].size() <<endl;
    }

    for (int centroidIndex = 0; centroidIndex < this->numOfClusters; centroidIndex++){
        cout <<(NumCDistType)silhouettes[centroidIndex].size() <<endl;
        meanSilhouettes = std::accumulate(silhouettes[centroidIndex].begin(), silhouettes[centroidIndex].end(), 0.0) / (NumCDistType)silhouettes[centroidIndex].size(); 
        overallSilhouettes[centroidIndex] = meanSilhouettes;
    }
    // get total mean of silouettes
    meanSilhouettes = std::accumulate(overallSilhouettes.begin(), overallSilhouettes.end(), 0.0) / (NumCDistType)overallSilhouettes.size();
    overallSilhouettes.push_back(meanSilhouettes);
    

    clock_t end = clock();
    cout <<"SILHOUETTE TIME [" << ((double) (end - start) / CLOCKS_PER_SEC) <<"]"<<endl;
    cout << "SILHOUETTE: [" << overallSilhouettes[overallSilhouettes.size()-1] << "]"<<endl;
    cout << "SILHOUETTE: [" << sumSi/this->numOfPoints << "]"<<endl;
    // for (int i = 0; i < overallSilhouettes.size(); i++){
    //     cout << overallSilhouettes[i] << ", ";
    // }
    
    return overallSilhouettes;
}

template <typename NumCDataType> 
vector<NumCDistType> Kmedians<NumCDataType>::getSilhouettes(){
    ExhaustiveKnn<NumCDataType>* knnEstimator = new ExhaustiveKnn<NumCDataType>(2);
    knnEstimator->fit(this->centroids);
    // Results* results = knnEstimator->predict_knn(this->data);
    Results* results = this->lastResults;

    // find the seconde nearest neighbour to fix results for the silouette
    NumCIndexType centroidIndex;
    NumCDistType  dist;
    Results* knnResults;
    for (int resultIndex = 0; resultIndex < results->resultsIndexArray.getRows(); resultIndex++){

        if (results->resultsIndexArray.getElement(resultIndex,1) == -1){
            knnResults = knnEstimator->predict_knn(this->data->getVector(resultIndex));
            // add its results (centroids) to the total results
            centroidIndex = knnResults->resultsIndexArray.getElement(0,0);
            dist = knnResults->resultsDistArray.getElement(0,0);
            if ( centroidIndex != results->resultsIndexArray.getElement(resultIndex,0) ){
                results->resultsIndexArray.addElement(centroidIndex, resultIndex, 1);
                results->resultsDistArray.addElement(dist, resultIndex, 1);
            }
            // add second nearest centroid
            centroidIndex = knnResults->resultsIndexArray.getElement(0,1);
            dist = knnResults->resultsDistArray.getElement(0,1);
            if ( centroidIndex != results->resultsIndexArray.getElement(resultIndex,0) ){
                results->resultsIndexArray.addElement(centroidIndex, resultIndex, 1);
                results->resultsDistArray.addElement(dist, resultIndex, 1);
            }
            delete knnResults;
        }
    }

    vector <NumCDistType> silouettes = getSilhouettes(results);
    // delete results;
    delete knnEstimator;
    return silouettes;
}

template <typename NumCDataType> 
NumCDistType Kmedians<NumCDataType>::getObjectiveCost(Results* results){
    NumCDistType sum = (NumCDistType)0;
    for (int i = 0; i < results->resultsDistArray.getRows(); i++){
        sum += results->resultsDistArray.getElement(i, 0);
    }
    return sum;
}

template <typename NumCDataType> 
NumCDistType Kmedians<NumCDataType>::getObjectiveCost(){
    // ExhaustiveKnn<NumCDataType>* knnEstimator = new ExhaustiveKnn<NumCDataType>(2);
    // knnEstimator->fit(this->centroids);
    // Results* results = knnEstimator->predict_knn(this->data);
    Results* results = this->lastResults;
    NumCDistType cost = getObjectiveCost(results);
    // delete results;
    // delete knnEstimator;
    return cost;
}

template <typename NumCDataType> 
void Kmedians<NumCDataType>::medianCentroidsUpdate(Results* results){

    std::vector<NumCIndexType> medianVector;
    medianVector.reserve(this->numOfPoints / this->numOfClusters);
    NumCDataType medianElement = 0;
    NumCDataType median = 0;
    int size = 0;

    for (int centroidIndex = 0; centroidIndex < this->numOfClusters; centroidIndex++){

        for (int dimension = 0; dimension < this->numOfDimensions; dimension++){  

            medianVector.clear();
            for (int resultsIndex = 0; resultsIndex < results->resultsIndexArray.getRows(); resultsIndex++){
                if ( results->resultsIndexArray.getElement(resultsIndex, 0) ==  centroidIndex){
                    medianElement = this->data->getElement(resultsIndex, dimension);
                    medianVector.push_back(medianElement);
                }
            }
            // calculate median for that dimension
            size = medianVector.size();
            if (size != 0){
                // get the upper boud of the index (index start at 0)
                std::sort(medianVector.begin(), medianVector.end());
                median = medianVector[size/2];
                centroids->addElement( median, centroidIndex, dimension);      
            }
        }
    }
}

template <typename NumCDataType> 
void Kmedians<NumCDataType>::fit_transform(NumC<NumCDataType>* trainData, ClusteringType clusteringType) {
    fit(trainData);
    transform(clusteringType);
}


template <typename NumCDataType> 
void Kmedians<NumCDataType>::transform(ClusteringType clusteringType){

    if (clusteringType == LLOYDS_CLUSTERING) {
        transform_LLOYDS_CLUSTERING();
    } else if (clusteringType == LSH_CLUSTERING) {
        transform_LSH_CLUSTERING();
    } else if (clusteringType == HC_CLUSTERING) {
        transform_HC_CLUSTERING();
    }

}

template <typename NumCDataType> 
void Kmedians<NumCDataType>::transform_LLOYDS_CLUSTERING(){

    NumCDistType prev_objectiveCost = numeric_limits<NumCDistType>::max();
    NumCDistType new_objectiveCost;
    NumCDistType objectiveError = numeric_limits<NumCDistType>::max();
    Results* results;

    // start clock for trasform
    clock_t start_median, end_median;
    // init centroids
    this->kmeansInit();

    ExhaustiveKnn<NumCDataType>* knnEstimator = new ExhaustiveKnn<NumCDataType>(2);
    knnEstimator->fit(this->centroids);

    clock_t start = clock();
    for (int i = 0; i < this->maxIterations; i++){
        
        results = knnEstimator->predict_knn(this->data);
        cout <<endl<<"KNN TIME [" << results->executionTime <<"]"<<endl; 

        // find the median for each centroid
        start_median = clock();
        medianCentroidsUpdate(results);
        end_median = clock();
        cout <<"MEDIAN TIME [" << ((double) (end_median - start_median) / CLOCKS_PER_SEC) <<"]"<<endl;

        new_objectiveCost = getObjectiveCost(results);
        cout << "COST: [" << new_objectiveCost << "] ERROR: [" << objectiveError<<"]" <<endl;

        this->lastResults = results;
        // if prev = new then optimization has converged
        // by 0.1%
        objectiveError = abs(new_objectiveCost - prev_objectiveCost) / prev_objectiveCost;
        prev_objectiveCost = new_objectiveCost;
        if (objectiveError < this->error || i >= this->maxIterations-1){
            this->transformTime = ((double) (clock() - start) / CLOCKS_PER_SEC);
            cout << endl<<"Kmedians Converged in [" << i+1 << "] iterations and in time ["<< this->transformTime << "]"<<endl;
            break;
        }
        delete results;
    }
    delete knnEstimator;
    getSilhouettes();
}

template <typename NumCDataType> 
void Kmedians<NumCDataType>::transform_HC_CLUSTERING(){

    NumCDistType prev_objectiveCost = numeric_limits<NumCDistType>::max();
    NumCDistType new_objectiveCost;
    NumCDistType objectiveError = numeric_limits<NumCDistType>::max();
    Results* results;

    // start clock for trasform
    clock_t start_median, end_median;
    // init centroids
    this->kmeansInit();

    HyperCube<NumCDataType>* hcEstimator = new HyperCube<NumCDataType>;
    hcEstimator->fit_transform(this->data, this->d);

    clock_t start = clock();
    for (int i = 0; i < this->maxIterations; i++){

        results = hcEstimator->reverse_assignment(this->centroids, this->M, this->probes);
        cout <<endl<<"HC TIME [" << results->executionTime <<"]"<<endl; 

        // find the median for each centroid
        start_median = clock();
        medianCentroidsUpdate(results);
        end_median = clock();
        cout <<"MEDIAN TIME [" << ((double) (end_median - start_median) / CLOCKS_PER_SEC) <<"]"<<endl;

        new_objectiveCost = getObjectiveCost(results);
        cout << "COST: [" << new_objectiveCost << "] ERROR: [" << objectiveError<<"]" <<endl;

        this->lastResults = results;
        // if prev = new then optimization has converged
        // by 0.1%
        objectiveError = abs(new_objectiveCost - prev_objectiveCost) / prev_objectiveCost;
        prev_objectiveCost = new_objectiveCost;
        if (objectiveError < this->error || i >= this->maxIterations-1){
            this->transformTime = ((double) (clock() - start) / CLOCKS_PER_SEC);
            cout << endl<<"Kmedians Converged in [" << i+1 << "] iterations and in time ["<< this->transformTime << "]"<<endl;
            break;
        }
        delete results;
    }
    delete hcEstimator;
    getSilhouettes();
}

template <typename NumCDataType> 
void Kmedians<NumCDataType>::transform_LSH_CLUSTERING(){

    NumCDistType prev_objectiveCost = numeric_limits<NumCDistType>::max();
    NumCDistType new_objectiveCost;
    NumCDistType objectiveError = numeric_limits<NumCDistType>::max();
    Results* results;

    // start clock for trasform
    clock_t start_median, end_median;
    // init centroids
    this->kmeansInit();

    LSHashing<NumCDataType>* lshEstimator = new LSHashing<NumCDataType>(this->L, this->k);
    lshEstimator->fit_transform(this->data);

    clock_t start = clock();
    for (int i = 0; i < this->maxIterations; i++){

        results = lshEstimator->reverse_assignment(this->centroids);
        cout <<endl<<"LSH TIME [" << results->executionTime <<"]"<<endl; 

        // find the median for each centroid
        start_median = clock();
        medianCentroidsUpdate(results);
        end_median = clock();
        cout <<"MEDIAN TIME [" << ((double) (end_median - start_median) / CLOCKS_PER_SEC) <<"]"<<endl;

        new_objectiveCost = getObjectiveCost(results);
        cout << "COST: [" << new_objectiveCost << "] ERROR: [" << objectiveError<<"]" <<endl;

        this->lastResults = results;
        // if prev = new then optimization has converged
        // by 0.1%
        objectiveError = abs(new_objectiveCost - prev_objectiveCost) / prev_objectiveCost;
        prev_objectiveCost = new_objectiveCost;
        if (objectiveError < this->error || i >= this->maxIterations-1){
            this->transformTime = ((double) (clock() - start) / CLOCKS_PER_SEC);
            cout << endl<<"Kmedians Converged in [" << i+1 << "] iterations and in time ["<< this->transformTime << "]"<<endl;
            break;
        }
        delete results;
    }
    delete lshEstimator;
    getSilhouettes();
}

#include "../include/pandac.h"
int main(){
    ConfigurationData configurationData; 
    Kmedians<int> kmeans(configurationData);


    NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte", 60000);
    // NumC<int>::print(inputData->getVector(0));
    // NumC<int>::printSparse(inputData->getVector(1));


    NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte", 60000);
//     // NumC<int>::print(inputDatalabels->getVector(0));

    kmeans.fit(inputData);

//     NumC<int>* inputData_ = new NumC<int>(10, inputData->getCols(), true);
//     for (int i = 0; i < 10; i++){
//         inputData_->addVector(inputData->getVector(i), i);
//     }

    kmeans.transform(LLOYDS_CLUSTERING);
    // kmeans.transform(LSH_CLUSTERING);

    // std::vector<Results*> res;
    // res = kmeans.getResults();
    // for (int i = 0; i < res.size(); i++){
    //     ResultsComparator::print(res[i], inputDatalabels);
    //     delete res[i];
    // }
    
//     // std::vector<int> ve;
//     // ve.reserve(10);
//     // ve.push_back(-1);
//     // ve.push_back(45);
//     // for (int i = 0; i < ve.size(); i++){
//     //     cout << ve[i] << endl;
//     // }
    

//     // Results* results;
    
//     // ResultsComparator::print(results, inputDatalabels);
//     // delete results;

    // delete inputData_;
    delete inputData;
    delete inputDatalabels;

}
