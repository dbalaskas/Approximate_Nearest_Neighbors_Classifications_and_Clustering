#include "../include/prediction_results.h"
#include "../include/numc.h"

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <iomanip>


using namespace std;


ResultsComparator::ResultsComparator(int size){
    this->numOfBestResults = size;
}

ResultsComparator::~ResultsComparator(){
}

// Results& Results::operator=(Results other_results){
    
//     this->numOfBestResults  = other_results.getNumOfResults();
//     this->executionTime     = other_results.executionTime;
//     this->resultsIndexArray = other_results.resultsIndexArray;
//     this->resultsDistArray  = other_results.resultsDistArray;

//     return *this;
// }

// void ResultsComparator::print(){

//     cout << "Best Results [" << this->numOfBestResults << "]  Time [" << this->executionTime << "]:\n";
//     for (int i = 0; i < this->numOfBestResults; i++){
//         cout << "Index: " << this->resultsIndexArray.getElement(0,i) << setw(15) 
//              << " Distance: " << this->resultsDistArray.getElement(0,i) <<endl;
//     }

// }

void ResultsComparator::print(Results* results, NumC<int>* labels){

    int index;
    int numOfQueries = results->resultsDistArray.getRows();
    cout <<endl<<"**************************************************"<<endl;
    cout << "*****Best Results of top [" << results->resultsDistArray.getCols() << "]  Time [" << results->executionTime << "]*****"<<endl;
    cout <<"**************************************************"<<endl;
    for (int query = 0; query < numOfQueries; query++){

        cout <<endl<< "Query ["<< query<< "]"<<endl;
        for (int i = 0; i < results->resultsDistArray.getCols(); i++){
            index = results->resultsIndexArray.getElement(query,i);
            cout << "Index: " << index << setw(15) 
                << " Distance: " << results->resultsDistArray.getElement(query,i) << setw(15) 
                << " Label: "<< labels->getVector(index).vector[0] << endl;
        }

    }
    

}


// std::vector<ResultIndex> Results::getResultList(){
//     return resultList;
// }

// double ResultsComparator::getTime(){
//     return this->executionTime;
// }

int ResultsComparator::getNumOfResults(){
    return this->numOfBestResults;
}

// void Results::result_swap(ResultIndex* res1, ResultIndex* res2){

//     // cout << res1-dist << " " << res2.dist <<"SWAP\n";
//     ResultIndex temp(res1->index, res1->dist);
//     res1->index = res2->index;
//     res1->dist  = res2->dist; 
//     res2->index = temp.index;
//     res2->dist  = temp.dist;
//     // cout << res1.dist << " " << res2.dist <<"SWAP\n";

// }

// void ResultsComparator::setTime(double time){
//     this->executionTime = time;
// }

int ResultsComparator::addResult(NumCIndexType index, NumCDistType dist){

    // check if index exists already in priority queue
    if (indexSet.insert(index).second){
        ResultIndex result(index, dist);
        priorityQueue.push(result);
    }
    return 0;
    // ResultIndex point(index, dist);
    // // add and keep the best num of results elements
    // if (this->numOfResults > 0){

    //     int last_result = numOfResults-1;

    //     // if result list is full then check if the last element of the list is greater then the result we
    //     // want to insert
    //     if (this->numOfResults == this->numOfBestResults){
    //         // if it is grater then swap it
    //         if (this->resultList[last_result].dist > point.dist){
    //             result_swap(&(this->resultList[last_result]), &(point));
    //         }
    //         else{
    //             return 0;
    //         }

    //     }
    //     else{
    //         this->resultList.push_back(point);
    //         this->numOfResults++;
    //         last_result = numOfResults-1;
    //     }
    //     // iterate and swap the smaller elements to the top of the list
    //     while(last_result > 0){
    //         if (this->resultList[last_result-1].dist > this->resultList[last_result].dist){
    //             // cout << this->resultList[last_result-1].dist << " " << this->resultList[last_result].dist  <<"SWAP\n";
    //             result_swap(&(this->resultList[last_result-1]), &(this->resultList[last_result]));
    //             last_result--;
    //         }
    //         else{
    //             return 0;
    //         }
    //     }
    // }
    // else{
    //     this->resultList.push_back(point);
    //     this->numOfResults++;
    // }

}

Results* ResultsComparator::getResults(){

    // if comparatorr is initialized for range search then 
    // the num of best resutls equal with the number of all the results
    if(this->numOfBestResults == 0){
        this->numOfBestResults = priorityQueue.size();
    }


    Results* results = new Results;
    results->resultsIndexArray = NumC<NumCIndexType>(1, this->numOfBestResults);
    results->resultsDistArray  = NumC<NumCDistType>(1, this->numOfBestResults);
    results->executionTime = 0.0;
    // results->executionTimeArray  = NumC<double>(this->numOfBestResults, 1);
    // NumC<NumCIndexType> resultsDistArray(1, this->numOfBestResults);

    int resultsFilled = 0;

    // add elements sorted in results numc array
    while (priorityQueue.empty() == false && resultsFilled < this->numOfBestResults){
        results->resultsIndexArray.addElement(priorityQueue.top().index, 0, resultsFilled);
        results->resultsDistArray.addElement(priorityQueue.top().dist, 0, resultsFilled);
        resultsFilled++;
        priorityQueue.pop();
    }

    return results;
}



bool RA_ResultsComparator::checkIndex(NumCIndexType index){
    if (cluster_map.count(index)>0)
        return true;
    else 
        return false;
}

RA_ResultIndex RA_ResultsComparator::getResult(NumCIndexType index){
    return cluster_map[index];
}

NumCIndexType RA_ResultsComparator::getResultsSize(){
    return (NumCIndexType)cluster_map.size();
}

int RA_ResultsComparator::addResult(NumCIndexType index, NumCIndexType cluster_index, NumCDistType dist){
    RA_ResultIndex result;
    result.first_cluster = cluster_index;
    result.first_dist  = dist;
    result.second_cluster = -1;
    result.second_dist = std::numeric_limits<NumCDistType>::max();
    cluster_map[index] = result;
    return 0;
}

int RA_ResultsComparator::addResultConflict(NumCIndexType index, NumCIndexType cluster_index, NumCDistType dist){

    if (dist < cluster_map[index].first_dist){
        // replace 1st cluster with the new one and move 1st to 2nd
        cluster_map[index].second_cluster = cluster_map[index].first_cluster;
        cluster_map[index].second_dist    = cluster_map[index].first_dist;
        cluster_map[index].first_cluster  = cluster_index;
        cluster_map[index].first_dist     = dist;

    } else if (dist >= cluster_map[index].first_dist && dist < cluster_map[index].second_dist) {
        // replace only second custer with new one
        cluster_map[index].second_cluster  = cluster_index;
        cluster_map[index].second_dist     = dist;
    }
    return 0;
}

Results* RA_ResultsComparator::getResults(){
    // fill with -1 indexes that do not exist in map

    Results* results = new Results;
    results->resultsIndexArray = NumC<NumCIndexType>(this->numOfBestResults, 2);
    results->resultsDistArray  = NumC<NumCDistType>(this->numOfBestResults, 2);
    // results->executionTimeArray  = NumC<double>(this->numOfBestResults, 1);
    
    // fill both arrays with -1 for the points that are not found during reverse assignment
    results->resultsIndexArray.fill(-1);
    results->resultsDistArray.fill(-1);
    for (std::map<int, RA_ResultIndex>::iterator iterator = cluster_map.begin(); iterator != cluster_map.end(); iterator++){

        std::cout << iterator->first << " => " << iterator->second.first_cluster << '\n';
        // fill the results arrays
        results->resultsIndexArray.addElement( iterator->second.first_cluster, iterator->first, 0);
        results->resultsIndexArray.addElement( iterator->second.second_cluster, iterator->first, 1);
        results->resultsDistArray.addElement( iterator->second.first_dist, iterator->first, 0);
        results->resultsDistArray.addElement( iterator->second.second_dist, iterator->first, 1);

    }

    return results;
}


/////////////// TEST ///////////////
// int main(){
// // 
    
//     std::map<int, int> mapp;

//     mapp[2] = 2;


//     RA_ResultsComparator ra(11);
//     cout << "travel map"<< ra.getResultsSize()<< endl;
//     ra.addResult(10, 0, 10.2);
//     ra.addResult(9, 3, 10.2);
//     ra.addResult(5, 5, 10.2);
//     ra.addResult(10, 0, 10.2);
//     ra.addResult(10, 0, 1000);
//     ra.addResult(1000, 0, 10.2);
//     cout << ra.checkIndex(10) <<endl;
//     cout << ra.getResult(10).first_dist <<endl;
//     ra.addResultConflict(10, 1, 10.3);
//     cout << ra.getResult(10).second_cluster <<endl;
//     cout << ra.getResult(10).first_dist <<endl;
//     cout << ra.getResult(10).second_dist <<endl;
//     cout << "travel map"<< ra.getResultsSize()<< endl;
//     // for (std::map<int, RA_ResultIndex>::iterator i = ra.cluster_map.begin(); i != ra.cluster_map.end(); i++){
//     //     std::cout << i->first << " => " << i->second.first_cluster << '\n';
//     // }
//     // Results* res = ra.getResults();

//     // res->resultsIndexArray.print();
//     // res->resultsDistArray.print();
//     // delete res;

// }