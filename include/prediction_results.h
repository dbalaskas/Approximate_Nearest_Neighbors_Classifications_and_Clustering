#ifndef PRED_RES_H
#define PRED_RES_H

#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <queue>
#include <iostream>

#include "../include/numc.h"

typedef struct ResultIndex{

    ResultIndex() {}
    ResultIndex(int index, double dist) : index(index), dist(dist) {}
    int index;
    double dist;

} ResultIndex;

class CompareResults{
    public:
    bool operator() (ResultIndex res1, ResultIndex res2){
        return res1.dist > res2.dist;
    }
};

class Results{

    private:
        int numOfResults;
        int numOfBestResults;
        std::vector<ResultIndex> resultList;
        double executionTime;
        void result_swap(ResultIndex* res1, ResultIndex* res2);

        std::priority_queue <ResultIndex, std::vector<ResultIndex>, CompareResults > pq;

    public:
        Results() {}
        Results(int size);
        ~Results();

        int addResult(ResultIndex resultIndex);
        void setTime(double time);

        double getTime();
        std::vector<ResultIndex> getResultList();
        int getNumOfResults();
        
        void print();
        void print(NumC<int>* labels, bool checkLabels=0);


};



#endif