#include "../include/numc.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cstring>

using namespace std;


NumC::NumC(int numOfRows, int numOfCols){
    // set matrix dimensions
    this->numOfRows = numOfRows;
    this->numOfCols = numOfCols;

    // allocate memory for the 2d matrix
    this->data = (VectorDataType*)malloc(this->numOfRows * this->numOfCols * sizeof(VectorDataType));
    memset(this->data, 0, this->numOfRows * this->numOfCols * sizeof(VectorDataType));
}


NumC::~NumC(){

    // deallocate the matrix memory
    free(this->data);

}

// get the vector of a column
Vector NumC::getVector(int index){

    Vector vector;
    vector.vector = (this->data + index*this->numOfRows);
    vector.size = this->numOfCols;
    return vector;

}

int NumC::getRows(){
    return numOfRows;
}

int NumC::getCols(){
    return numOfCols;
}

void NumC::addElement(VectorDataType element, int row, int col){
    this->data[this->numOfRows*row + col] = element;
}

void NumC::print(){

    for (int i = 0; i < this->numOfRows; i++){
        for (int j = 0; j < this->numOfCols; j++){
            cout << data[i*this->numOfRows + j] << ", ";
        }
        cout << "\n";
    }
    

}

int main(){

    NumC nn(4,33);
    for (int i = 0; i < nn.getRows(); i++){
        for (int j = 0; j < nn.getCols(); j++){
            nn.addElement(i, i, j);
        }
        
    }
    nn.print();
}