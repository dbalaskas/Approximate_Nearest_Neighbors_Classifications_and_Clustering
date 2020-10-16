#ifndef PRED_RES_H
#define PRED_RES_H

#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <queue>
#include <iostream>

#include "./numc.h"

typedef struct ResultIndex{

    ResultIndex() {};
    ResultIndex(int index, double dist) : index(index), dist(dist) {};
    int index;
    double dist;

} ResultIndex;


typedef struct Results{

    Results() {};
    Results(NumCIndexType resultsRows, NumCIndexType resultsCol): resultsIndexArray(resultsRows, resultsCol), resultsDistArray(resultsRows, resultsCol) {};
    NumC<NumCIndexType> resultsIndexArray;
    NumC<NumCDistType> resultsDistArray;
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

    public:
        ResultsComparator() {};
        ResultsComparator(int size);
        ~ResultsComparator();
        static void print(Results* results, NumC<int>* labels);


        int addResult(int index, double dist);

        Results* getResults();
        std::vector<ResultIndex> getResultList();
        int getNumOfResults();
        
        void print();
};



#endif