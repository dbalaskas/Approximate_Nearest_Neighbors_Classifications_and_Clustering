#ifndef PRED_RES_H
#define PRED_RES_H

#include <vector>
#include <queue>
#include <unordered_set>
#include <map> 

#include "./numc.h"

typedef struct ResultIndex{

    ResultIndex() {};
    ResultIndex(int index, double dist) : index(index), dist(dist) {};
    int index;
    double dist;

} ResultIndex;


typedef struct Results{

    Results() {};
    Results(NumCIndexType resultsRows, NumCIndexType resultsCol): resultsIndexArray(resultsRows, resultsCol), resultsDistArray(resultsRows, resultsCol), executionTimeArray(resultsRows, 1) {};
    NumC<NumCIndexType> resultsIndexArray;
    NumC<NumCDistType> resultsDistArray;
    NumC<double> executionTimeArray;
    double executionTime;

    // Results(){ 
    //     resultsIndexArray = NumC<NumCIndexType>(); 
    // }
    // ~Results(){}
    // Results(Results& other_results){ 

    //     this->resultsIndexArray = other_results.resultsIndexArray;
    //     this->resultsDistArray  = other_results.resultsDistArray;
    //     this->executionTime     = other_results.executionTime;
    // }
    // Results& operator=(Results& other_results){
    //     // delete current data
    //     this->resultsIndexArray = other_results.resultsIndexArray;
    //     this->resultsDistArray  = other_results.resultsDistArray;
    //     this->executionTime     = other_results.executionTime;
    //     return *this;
    // }
    // Results& operator=(Results&& other_results) noexcept{ // move assignment
    //     this->resultsIndexArray = other_results.resultsIndexArray;
    //     this->resultsDistArray  = other_results.resultsDistArray;
    //     this->executionTime     = other_results.executionTime;
    //     return *this;
    // }

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
        ResultsComparator() {};
        ResultsComparator(int size);
        ~ResultsComparator();
        static void print(Results* results, NumC<int>* labels);


        int addResult(int index, double dist);

        Results* getResults();
        // std::vector<ResultIndex> getResultList();
        int getNumOfResults();
        
        void print();
};

typedef struct RA_ResultIndex {
    int first_cluster;
    double first_dist;
    int second_cluster;
    double second_dist;
} RA_ResultIndex;

class RA_ResultsComparator{

    private:
        // int numOfBestResults;
        // std::priority_queue <ResultIndex, std::vector<ResultIndex>, Compare > priorityQueue;
        // std::unordered_set<NumCIndexType> indexSet;
        std::map<int, RA_ResultIndex> cluster_map;

    public:
        RA_ResultsComparator() {};
        RA_ResultsComparator(int size);
        ~RA_ResultsComparator();
        // static void print(Results* results, NumC<int>* labels);


        int addResult(int index, double dist);

        Results* getResults();
        // std::vector<ResultIndex> getResultList();
        int getNumOfResults();
        
        void print();
};

#endif