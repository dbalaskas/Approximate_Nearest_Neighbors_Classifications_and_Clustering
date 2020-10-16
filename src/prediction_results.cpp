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

int ResultsComparator::addResult(int index, double dist){
    ResultIndex result(index, dist);
    priorityQueue.push(result);
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

    Results* results = new Results;
    results->resultsIndexArray = NumC<NumCIndexType>(1, this->numOfBestResults);
    results->resultsDistArray  = NumC<NumCDistType>(1, this->numOfBestResults);
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




/////////////// TEST ///////////////
// int main(){

//     NumC nn(6,33);
//     for (int i = 0; i < nn.getRows(); i++){
//         for (int j = 0; j < nn.getCols(); j++){
//             nn.addElement(i, i, j);
//         }
        
//     }
//     // nn.print();

//     // Vector v = nn.getVector(3);

//     Results res(3);
//     res.print();
//     ResultIndex resu(12, 66);
//     res.addResult(resu);
//     res.print();
//     resu.dist = 10;
//     res.addResult(resu);
//     res.print();
//     resu.dist = 100;
//     res.addResult(resu);
//     res.print();
//     resu.dist = 1;
//     res.addResult(resu);
//     res.print();
//     res.addResult(resu);
//     resu.dist = 2;
//     res.addResult(resu);
//     res.addResult(resu);
//     res.print(); 

// }