#include "../include/prediction_results.h"
#include "../include/numc.h"

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>


using namespace std;


Results::Results(int size){
    this->numOfResults = 0;
    this->numOfBestResults = size;
    this->executionTime = 0.0;
    this->resultList.reserve(size+1);

}

Results::~Results(){
    this->numOfResults = 0;
    this->resultList.clear();
}


void Results::print(){

    int numOfResults_ = this->numOfBestResults;
    if (this->numOfResults < this->numOfBestResults)
        numOfResults_ = this->numOfResults;


    cout << "Best Results [" << numOfResults_ << "]  Time [" << this->executionTime << "]:\n";
    for (int i = 0; i < numOfResults_; i++){
        cout << "Index: "  << this->resultList[i].index << setw(15) <<
        " Distance: " << this->resultList[i].dist <<endl;
    }

}

void Results::print(NumC<int>* labels, bool checkLabels){

    int numOfResults_ = this->numOfBestResults;
    if (this->numOfResults < this->numOfBestResults)
        numOfResults_ = this->numOfResults;

    int index;

    cout << "Best Results [" << numOfResults_ << "]  Time [" << this->executionTime << "]:\n";
    for (int i = 0; i < numOfResults_; i++){
        index = this->resultList[i].index;
        cout << "Index: " << index << setw(15) 
             << " Distance: " << this->resultList[i].dist << setw(15) 
             << " Label: "<< labels->getVector(index).vector[0] << endl;
        cout << "pqIndex: " << pq.top().index <<endl;
        pq.pop();

    }

}


std::vector<ResultIndex> Results::getResultList(){
    return resultList;
}

double Results::getTime(){
    return this->executionTime;
}

int Results::getNumOfResults(){
    return this->numOfResults;
}

void Results::result_swap(ResultIndex* res1, ResultIndex* res2){

    // cout << res1-dist << " " << res2.dist <<"SWAP\n";
    ResultIndex temp(res1->index, res1->dist);
    res1->index = res2->index;
    res1->dist  = res2->dist; 
    res2->index = temp.index;
    res2->dist  = temp.dist;
    // cout << res1.dist << " " << res2.dist <<"SWAP\n";

}

void Results::setTime(double time){
    this->executionTime = time;
}

int Results::addResult(ResultIndex point){
    pq.push(point);

    // add and keep the best num of results elements
    if (this->numOfResults > 0){

        int last_result = numOfResults-1;

        // if result list is full then check if the last element of the list is greater then the result we
        // want to insert
        if (this->numOfResults == this->numOfBestResults){
            // if it is grater then swap it
            if (this->resultList[last_result].dist > point.dist){
                result_swap(&(this->resultList[last_result]), &(point));
            }
            else{
                return 0;
            }

        }
        else{
            this->resultList.push_back(point);
            this->numOfResults++;
            last_result = numOfResults-1;
        }
        // iterate and swap the smaller elements to the top of the list
        while(last_result > 0){
            if (this->resultList[last_result-1].dist > this->resultList[last_result].dist){
                // cout << this->resultList[last_result-1].dist << " " << this->resultList[last_result].dist  <<"SWAP\n";
                result_swap(&(this->resultList[last_result-1]), &(this->resultList[last_result]));
                last_result--;
            }
            else{
                return 0;
            }
        }
    }
    else{
        this->resultList.push_back(point);
        this->numOfResults++;
    }


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