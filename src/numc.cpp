#include "../include/numc.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <complex>
#include <cmath>

using namespace std;

template class NumC<int>;
template class NumC<double>;

template <typename NumCDataType>
NumC<NumCDataType>::NumC() {
    // set matrix dimensions
    this->numOfRows = 0;
    this->numOfCols = 0;
    this->size = 0;

    // allocate memory for the 2d matrix
    this->data = NULL;
}

template <typename NumCDataType>
NumC<NumCDataType>::NumC(int numOfRows, int numOfCols){
    // set matrix dimensions
    this->numOfRows = numOfRows;
    this->numOfCols = numOfCols;
    this->size = this->numOfRows * this->numOfCols ;

    // allocate memory for the 2d matrix
    this->data = (NumCDataType*)malloc(this->size * sizeof(NumCDataType));
    memset(this->data, 0, this->size * sizeof(NumCDataType));
}

template <typename NumCDataType>
NumC<NumCDataType>& NumC<NumCDataType>::operator=(NumC<NumCDataType> other_numc){
    // delete current data
    free(this->data);
    
    // copy opreations
    this->numOfRows = other_numc.getRows();
    this->numOfCols = other_numc.getCols();
    this->size = this->numOfCols * this->numOfRows;

    this->data = (NumCDataType*)malloc(this->size * sizeof(NumCDataType));
    memset(this->data, 0, this->size * sizeof(NumCDataType)); 
    memcpy(this->data, other_numc.getData(), this->size* sizeof(NumCDataType));

    return *this;
}

template <typename NumCDataType>
NumC<NumCDataType>::~NumC(){

    // deallocate the matrix memory
    free(this->data);
}

// get the vector of a row.
template <typename NumCDataType>
Vector<NumCDataType> NumC<NumCDataType>::getVector(int index){

    Vector<NumCDataType> vector;
    vector.vector = (this->data + index*this->numOfCols);
    vector.size = this->numOfCols;
    return vector;
}

template <typename NumCDataType>
NumCDataType* NumC<NumCDataType>::getData(){
    return this->data;
}

template <typename NumCDataType>
int NumC<NumCDataType>::getRows(){
    return numOfRows;
}

template <typename NumCDataType>
int NumC<NumCDataType>::getCols(){
    return numOfCols;
}

template <typename NumCDataType>
void NumC<NumCDataType>::transpose(){

    // transpose algorithm
    NumCDataType* data_ = (NumCDataType*)malloc(this->size * sizeof(NumCDataType));
    memcpy(data_, this->data, this->size * sizeof(NumCDataType));

    int offset_i;
    int offset_j;
    for (int i = 0; i<this->size; i++) {
        offset_i = (int)(i / this->numOfRows);
        offset_j = (int)(i % this->numOfRows);
        data[i] = data_[this->numOfCols * offset_j + offset_i];
    }

    free(data_);

    int tempDim = this->numOfCols;
    this->numOfCols = this->numOfRows;
    this->numOfRows = tempDim;
}

template <typename NumCDataType>
void NumC<NumCDataType>::addElement(NumCDataType element, int row, int col){
    this->data[this->numOfCols*row + col] = element;
}

template <typename NumCDataType>
void NumC<NumCDataType>::appendVector(Vector<NumCDataType> vector){
    // NumC::print(vector);
    if( vector.size != this->numOfCols ){
        cout << "Wrong input size vector\n";
        return;
    }

    this->numOfRows++;
    this->size = this->numOfRows * this->numOfCols;

    this->data = (NumCDataType*)realloc(this->data, this->size * sizeof(NumCDataType));
    memcpy((this->data + (this->numOfRows-1)*this->numOfCols), vector.vector, vector.size * sizeof(NumCDataType));

}

template <typename NumCDataType>
void NumC<NumCDataType>::addVector(Vector<NumCDataType> vector, int index){
    // NumC::print(vector);
    if( vector.size != this->numOfCols ){
        cout << "Wrong input size vector\n";
        return;
    }
    
    memcpy((this->data + (this->numOfCols)*index), vector.vector, vector.size * sizeof(NumCDataType));
}

template <typename NumCDataType>
void NumC<NumCDataType>::print(){

    cout << "Numc matrix of shape [" << this->numOfRows << "," << this->numOfCols << "]\n";

    for (int i = 0; i < this->numOfRows; i++){
        for (int j = 0; j < this->numOfCols; j++){
            cout << data[i*this->numOfCols + j] << ", ";
        }
        cout << "\n";
    }

}

template <typename NumCDataType>
void NumC<NumCDataType>::print(Vector<NumCDataType> vector){

    cout << "Numc vector of shape [" << 1 << "," << vector.size << "]\n";

    for (int i = 0; i < vector.size; i++){
        cout << vector.vector[i] << ", ";
    }
    cout << "\n";

}

template <typename NumCDataType>
double NumC<NumCDataType>::dist(Vector<NumCDataType> v1, Vector<NumCDataType> v2, int d){

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