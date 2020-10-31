#ifndef PRED_RES_H
#define PRED_RES_H

#include <queue>
#include <unordered_set>
#include <map> 
#include <limits>

#include "./numc.h"

// Results for LSH/Hypercube

typedef struct ResultIndex{

    ResultIndex() {};
    ResultIndex(int index, double dist) : index(index), dist(dist) {};
    int index;
    double dist;

} ResultIndex;


typedef struct Results{

    Results() {};
    Results(NumCIndexType resultsRows, NumCIndexType resultsCol): 
        resultsIndexArray(resultsRows, resultsCol), 
        resultsDistArray(resultsRows, resultsCol), 
        executionTimeArray(resultsRows, 1) {};

    NumC<NumCIndexType> resultsIndexArray;
    NumC<NumCDistType> resultsDistArray;
    NumC<double> executionTimeArray;
    double executionTime;

} Results;

class Compare{
    public:
    bool operator() (ResultIndex res1, ResultIndex res2){
        return res1.dist > res2.dist;
    }
};

class ResultsComparator{

    private:
        int numOfBestResults;
        std::priority_queue <ResultIndex, std::vector<ResultIndex>, Compare > priorityQueue;
        std::unordered_set<NumCIndexType> indexSet; 

    public:
        ResultsComparator(): numOfBestResults{0} {};
        ResultsComparator(int size);
        ~ResultsComparator();
        static void print(Results* results, NumC<int>* labels);


        int addResult(NumCIndexType index, NumCDistType dist);

        Results* getResults();
        int getNumOfResults();
        
        void print();
};

// -----------------------------------------------------------------
// Results for clustering

typedef struct RA_ResultIndex {
    NumCIndexType first_cluster;
    NumCDistType first_dist;
    NumCIndexType second_cluster;
    NumCDistType second_dist;
} RA_ResultIndex;

class RA_ResultsComparator{

    private:
        int numOfBestResults;
        std::map<NumCIndexType, RA_ResultIndex> cluster_map;

    public:
        RA_ResultsComparator(int size): numOfBestResults{size} {};
        ~RA_ResultsComparator() {};
        bool checkIndex(NumCIndexType index);

        int addResult(NumCIndexType index, NumCIndexType cluster_index, NumCDistType dist);
        int addResultSecond(NumCIndexType index, NumCIndexType cluster_index, NumCDistType dist);
        int addResultConflict(NumCIndexType index, NumCIndexType cluster_index, NumCDistType dist);
        RA_ResultIndex getResult(NumCIndexType index);
        NumCIndexType getResultsSize();

        Results* getResults();
        int getNumOfResults();
        
        void print();
};

#endif