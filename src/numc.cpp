#include "../include/numc.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <complex>
#include <cmath>

using namespace std;

NumC::NumC() {
    // set matrix dimensions
    this->numOfRows = 0;
    this->numOfCols = 0;
    this->size = 0;

    // allocate memory for the 2d matrix
    this->data = NULL;
}

NumC::NumC(int numOfRows, int numOfCols){
    // set matrix dimensions
    this->numOfRows = numOfRows;
    this->numOfCols = numOfCols;
    this->size = this->numOfRows * this->numOfCols ;

    // allocate memory for the 2d matrix
    this->data = (VectorDataType*)malloc(this->size * sizeof(VectorDataType));
    memset(this->data, 0, this->size * sizeof(VectorDataType));
}

NumC& NumC::operator=(NumC other_numc){
            
    this->numOfRows = other_numc.getRows();
    this->numOfCols = other_numc.getCols();
    this->size = this->numOfCols * this->numOfRows;

    this->data = (VectorDataType*)malloc(this->size * sizeof(VectorDataType));
    memset(this->data, 0, this->size * sizeof(VectorDataType));
    memcpy(this->data, other_numc.getData(), this->size);

    return *this;
}

NumC::~NumC(){

    // deallocate the matrix memory
    free(this->data);

}

// get the vector of a column
Vector NumC::getVector(int index){

    Vector vector;
    vector.vector = (this->data + index*this->numOfCols);
    vector.size = this->numOfCols;
    return vector;

}
VectorDataType* NumC::getData(){
    return this->data;
}

int NumC::getRows(){
    return numOfRows;
}

int NumC::getCols(){
    return numOfCols;
}

void NumC::addElement(VectorDataType element, int row, int col){
    this->data[this->numOfCols*row + col] = element;
}

void NumC::addVector(Vector vector){
    NumC::print(vector);


    if( vector.size != this->numOfCols ){
        cout << "Wrong input size vector\n";
        return;
    }

    this->numOfRows++;
    this->size = this->numOfRows * this->numOfCols;

    this->data = (VectorDataType*)realloc(this->data, this->size * sizeof(VectorDataType));
    memcpy((this->data + (this->numOfRows-1)*this->numOfCols), vector.vector, vector.size * sizeof(VectorDataType));

}

void NumC::print(){

    cout << "Numc matrix of shape [" << this->numOfRows << "," << this->numOfCols << "]\n";

    for (int i = 0; i < this->numOfRows; i++){
        for (int j = 0; j < this->numOfCols; j++){
            cout << data[i*this->numOfCols + j] << ", ";
        }
        cout << "\n";
    }

}

void NumC::print(Vector vector){

    cout << "Numc vector of shape [" << 1 << "," << vector.size << "]\n";

    for (int i = 0; i < vector.size; i++){
        cout << vector.vector[i] << ", ";
    }
    cout << "\n";

}


double NumC::dist(Vector v1, Vector v2, int d){

    double dist = 0;
    // calculate manhattan distance if dimension = 1
    if (d == 1){
        for (int i = 0; i < v1.size; i++){
            dist += std::abs( v1.vector[i] - v2.vector[i] ); 
        }
        return dist;   
    }

    // calculate distance with p-norm
    for (int i = 0; i < v1.size; i++){
        dist += std::pow( v1.vector[i] - v2.vector[i], d ); 
    }
    dist = std::pow( dist, 1.0/d );
    return dist;

}

// int main(){

//     NumC nn(6,1);
//     NumC nn(6,33);
//     for (int i = 0; i < nn.getRows(); i++){
//         for (int j = 0; j < nn.getCols(); j++){
//             nn.addElement(i, i, j);
//         }
        
//     }
//     nn.print();

//     Vector v = nn.getVector(3);
//     NumC::print(v);

//     nn.addVector(v);
//     nn.print();

//     cout << NumC::dist(nn.getVector(5),nn.getVector(4),2);
//     cout << NumC::dist(nn.getVector(6),nn.getVector(4),2);
// }