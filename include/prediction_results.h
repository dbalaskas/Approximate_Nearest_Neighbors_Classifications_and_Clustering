#ifndef PRED_RES_H
#define PRED_RES_H

#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <iostream>

typedef struct PointIndex{

    PointIndex() {}
    PointIndex(int intex, double dist) : intex(intex), dist(dist) {}
    int intex;
    double dist;

} PointIndex;

typedef struct Results{

    Results() {}
    Results(int size) {this->resultList.reserve(size);}
    int numOfResults;
    std::vector<PointIndex> resultList;
    double time;

} Results;



#endif