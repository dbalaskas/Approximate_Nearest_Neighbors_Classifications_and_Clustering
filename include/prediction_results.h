#ifndef PRED_RES_H
#define PRED_RES_H

#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include "../include/numc.h"

typedef struct ResultIndex{

    ResultIndex() {}
    ResultIndex(int index, double dist) : index(index), dist(dist) {}
    int index;
    double dist;

} ResultIndex;

class Results{

    private:
        int numOfResults;
        int numOfBestResults;
        std::vector<ResultIndex> resultList;
        double executionTime;
        void result_swap(ResultIndex* res1, ResultIndex* res2);

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
        void print(NumC* labels, bool checkLabels=0);


};



#endif