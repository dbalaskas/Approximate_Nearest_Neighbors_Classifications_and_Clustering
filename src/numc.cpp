#include "../include/numc.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <complex>
#include <cmath>
#include <ctime>
#include <random>

using namespace std;

template <typename NumCDataType>
NumC<NumCDataType>::NumC() {
    // set matrix dimensions
    this->numOfRows = 0;
    this->numOfCols = 0;
    this->size = 0;
    this->isSparse_ = false;

    // allocate memory for the 2d matrix
    this->data = NULL;
    this->sparseData = NULL;
}

template <typename NumCDataType>
NumC<NumCDataType>::NumC(NumCIndexType numOfRows, NumCIndexType numOfCols, bool isSparse){
    // set matrix dimensions
    this->numOfRows = numOfRows;
    this->numOfCols = numOfCols;
    this->size = this->numOfRows * this->numOfCols ;
    this->isSparse_ = isSparse;

    // allocate memory for the 2d matrix
    this->data = (NumCDataType*)malloc(this->size * sizeof(NumCDataType));
    memset(this->data, 0, this->size * sizeof(NumCDataType));

    // allocate sparse memory
    if (isSparse){
        this->sparseData = (NumCIndexType*)malloc((this->size + this->numOfRows) * sizeof(NumCIndexType));
        memset(this->sparseData, 0, (this->size + this->numOfRows) * sizeof(NumCIndexType));
        for (int i = 1; i < (this->size + this->numOfRows); i++){
            this->sparseData[i] = this->numOfCols +1;
        }
        for (int i = 1; i < this->numOfRows; i++){
            this->sparseData[i*(this->numOfCols+1)] = 0;
        }
        
    } else {
        this->sparseData = NULL;
    }

}

template <typename NumCDataType>
NumC<NumCDataType>& NumC<NumCDataType>::operator=(NumC<NumCDataType> other_numc){
    // delete current data
    if (this->data != NULL) {
        free(this->data);
        this->data = NULL;
    }
    if (this->isSparse_) {
        free(this->sparseData);
        this->data = NULL;
    }
    
    // copy opreations
    this->numOfRows = other_numc.getRows();
    this->numOfCols = other_numc.getCols();
    this->size = this->numOfCols * this->numOfRows;

    this->data = (NumCDataType*)malloc(this->size * sizeof(NumCDataType));
    memset(this->data, 0, this->size * sizeof(NumCDataType)); 
    memcpy(this->data, other_numc.getData(), this->size* sizeof(NumCDataType));

    if (other_numc.isSparse()){
        this->sparseData = (NumCIndexType*)malloc((this->size + this->numOfRows) * sizeof(NumCIndexType));
        memset(this->sparseData, 0, (this->size + this->numOfRows) * sizeof(NumCIndexType));
        memcpy(this->sparseData, other_numc.getSparseData(), (this->size + this->numOfRows)* sizeof(NumCIndexType));
        for (int i = 1; i < (this->size + this->numOfRows); i++){
            this->sparseData[i] = this->numOfCols +1;
        }
        for (int i = 1; i < this->numOfRows; i++){
            this->sparseData[i*(this->numOfCols+1)] = 0;
        }
    } else {
        sparseData = NULL;
    }

    return *this;
}

template <typename NumCDataType>
NumC<NumCDataType>::~NumC(){

    // deallocate the matrix memory
    if (this->data != NULL) {
        free(this->data);
        this->data = NULL;
    }
    if (this->sparseData != NULL) {
        free(this->sparseData);
        this->sparseData = NULL;
    }
}

// get the vector of a row.
template <typename NumCDataType>
Vector<NumCDataType> NumC<NumCDataType>::getVector(NumCIndexType index){

    Vector<NumCDataType> vector;
    vector.vector = (this->data + index*this->numOfCols);
    vector.size = this->numOfCols;

    vector.isSparse_ = this->isSparse_;
    if(this->isSparse_){
        vector.sparseData = (this->sparseData + index*(this->numOfCols+1));
    }

    return vector;
}

template <typename NumCDataType>
bool NumC<NumCDataType>::isSparse(){
    return this->isSparse_;
}

template <typename NumCDataType>
NumCIndexType* NumC<NumCDataType>::getSparseData(){
    return this->sparseData;
}

template <typename NumCDataType>
NumCDataType* NumC<NumCDataType>::getData(){
    return this->data;
}

template <typename NumCDataType>
NumCIndexType NumC<NumCDataType>::getRows(){
    return numOfRows;
}

template <typename NumCDataType>
NumCIndexType NumC<NumCDataType>::getCols(){
    return numOfCols;
}
template <typename NumCDataType>
NumCDataType NumC<NumCDataType>::getElement(NumCIndexType row, NumCIndexType col){
    return this->data[this->numOfCols*row + col];
}

template <typename NumCDataType>
NumCDataType NumC<NumCDataType>::getLast(){
    return this->data[this->size-1];
}

template <typename NumCDataType>
NumCIndexType NumC<NumCDataType>::find(NumCDataType element){
    // search and find the first greater or equal element of the array
    for (NumCIndexType i = 0; i < this->size; i++){
        if (this->data[i] >= element)
            return i;
    }
    return this->size-1;
}


template <typename NumCDataType>
void NumC<NumCDataType>::random(NumCDataType maxValue){
    // fill with random values
    srand(time(NULL));

    std::random_device randomDevice; 
    std::mt19937 generator(randomDevice()); 
    std::uniform_int_distribution<NumCIndexType> distribution(0,maxValue-1);

    for (NumCIndexType i = 0; i < this->size; i++){
        // data[i] = (NumCDataType)(rand()%(NumCDataType)maxValue);
        data[i] = (NumCDataType)distribution(generator);
    }

}

template <typename NumCDataType>
NumCDataType NumC<NumCDataType>::max(){
    // get the max
    NumCDataType max = this->data[0];
    for (int i = 0; i < this->size; i++){
        if (this->data[i] > max)
            max = this->data[i];
    }
    return max;
}

template <typename NumCDataType>
void NumC<NumCDataType>::square(){
    // square all elements
    for (int i = 0; i < this->size; i++){
        this->data[i] = this->data[i] * this->data[i];
    }
}

template <typename NumCDataType>
void NumC<NumCDataType>::normalize(){
    // normalize all elements
    // get the max
    NumCDataType max = this->max();
    // devide by max
    for (int i = 0; i < this->size; i++){
        this->data[i] = this->data[i] / max;
    }
}

template <typename NumCDataType>
void NumC<NumCDataType>::cumulative(){
    // get the comulative
    // devide by max
    for (int i = 1; i < this->size; i++){
        this->data[i] = this->data[i] + this->data[i-1];
    }
}

template <typename NumCDataType>
NumCDataType NumC<NumCDataType>::sum(){
    // get the sum
    NumCDataType sum = (NumCDataType)0;
    for (int i = 0; i < this->size; i++){
        sum += this->data[i];
    }
    return sum;
}


template <typename NumCDataType>
void NumC<NumCDataType>::fill(NumCDataType fillValue){
    // fill fill value
    for (int i = 0; i < this->size; i++){
        this->data[i] = fillValue;
    }
}

template <typename NumCDataType>
void NumC<NumCDataType>::transpose(){

    // transpose algorithm
    NumCDataType* data_ = (NumCDataType*)malloc(this->size * sizeof(NumCDataType));
    memcpy(data_, this->data, this->size * sizeof(NumCDataType));

    NumCIndexType offset_i;
    NumCIndexType offset_j;
    for (NumCIndexType i = 0; i<this->size; i++) {
        offset_i = (NumCIndexType)(i / this->numOfRows);
        offset_j = (NumCIndexType)(i % this->numOfRows);
        data[i] = data_[this->numOfCols * offset_j + offset_i];
    }

    free(data_);

    NumCIndexType tempDim = this->numOfCols;
    this->numOfCols = this->numOfRows;
    this->numOfRows = tempDim;
}

template <typename NumCDataType>
void NumC<NumCDataType>::addElement(NumCDataType element, NumCIndexType row, NumCIndexType col){
    this->data[this->numOfCols*row + col] = element;

    // update sparse data
    if(this->isSparse_){
        if(element != 0){
            this->sparseData[(this->numOfCols+1)*row + this->sparseData[(this->numOfCols+1)*row] + 1] = col;
            this->sparseData[(this->numOfCols+1)*row]++;
        }
    }
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
void NumC<NumCDataType>::addVector(Vector<NumCDataType> vector, NumCIndexType index){
    // NumC::print(vector);
    if( vector.size != this->numOfCols ){
        cout << "Wrong input size vector " << index <<endl;
        return;
    }
    
    memcpy((this->data + (this->numOfCols)*index), vector.vector, vector.size * sizeof(NumCDataType));

    // update sparse data
    if(this->isSparse_){
        memcpy((this->sparseData + (this->numOfCols+1)*index), vector.sparseData, (vector.size + 1) * sizeof(NumCIndexType));
    }
}

template <typename NumCDataType>
void NumC<NumCDataType>::addArray(NumC<NumCDataType> array, NumCIndexType index){
    // NumC::print(vector);
    if( array.size != this->numOfCols ){
        cout << "Wrong input size vector " << index <<endl;
        return;
    }
    
    memcpy((this->data + (this->numOfCols)*index), array.getData(), array.getCols() * sizeof(NumCDataType));
}

template <typename NumCDataType>
void NumC<NumCDataType>::print(){

    cout << "Numc matrix of shape [" << this->numOfRows << "," << this->numOfCols << "]\n";

    for (NumCIndexType i = 0; i < this->numOfRows; i++){
        for (NumCIndexType j = 0; j < this->numOfCols; j++){
            cout << data[i*this->numOfCols + j] << ", ";
        }
        cout << "\n";
    }

}

template <typename NumCDataType>
void NumC<NumCDataType>::printSparse(Vector<NumCDataType> vector){

    cout << "Numc sparse vector of shape [" << 1 << "," << vector.size +1 << "]\n";

    for (NumCIndexType i = 0; i < vector.size+1; i++){
        cout << vector.sparseData[i] << ", ";
    }
    cout << "\n";

}

template <typename NumCDataType>
void NumC<NumCDataType>::print(Vector<NumCDataType> vector){

    cout << "Numc vector of shape [" << 1 << "," << vector.size << "]\n";

    for (NumCIndexType i = 0; i < vector.size; i++){
        cout << vector.vector[i] << ", ";
    }
    cout << "\n";

}

template <typename NumCDataType>
NumCDistType NumC<NumCDataType>::dist(Vector<NumCDataType> v1, Vector<NumCDataType> v2, NumCIndexType d){

    NumCDistType dist = 0;
    // calculate manhattan distance if dimension = 1
    if (d == 1){
        for (NumCIndexType i = 0; i < v1.size; i++){
            dist += std::abs( v1.vector[i] - v2.vector[i] ); 
        }
        return dist;   
    }

    // calculate distance with p-norm
    for (NumCIndexType i = 0; i < v1.size; i++){
        dist += std::pow( v1.vector[i] - v2.vector[i], d ); 
    }
    dist = std::pow( dist, 1.0/d );
    return dist;

}

template <typename NumCDataType>
NumCDistType NumC<NumCDataType>::distSparse(Vector<NumCDataType> v1, Vector<NumCDataType> v2, NumCIndexType d){
    // chech if both are sparse vectors
    if ((v1.isSparse_ == true) && (v2.isSparse_ == true)){

        NumCDistType dist = 0;
        NumCIndexType sparseElements1 = v1.sparseData[0];
        NumCIndexType sparseElements2 = v2.sparseData[0];
        NumCIndexType index1 = 1;
        NumCIndexType index2 = 1;
        // NumCIndexType index  = 1;
        // calculate manhattan distance if dimension = 1
        if (d == 1){
            // for (NumCIndexType i = 1; i < sparseElements1; i++){
            //     index = v1.sparseData[i];
            //     dist += std::abs( v1.vector[index] - v2.vector[index] ); 
            // }
            // for (NumCIndexType i = 1; i < sparseElements2; i++){
            //     index = v2.sparseData[i];
            //     dist += std::abs( v1.vector[index] - v2.vector[index] ); 
            // }
            // return dist;    
            while( (index1 <= sparseElements1 )|| (index2 <= sparseElements2)){
                if ( v1.sparseData[index1] < v2.sparseData[index2]){
                    dist += std::abs( v1.vector[v1.sparseData[index1]] - v2.vector[v1.sparseData[index1]] );
                    index1++; 
                }
                else if (v1.sparseData[index1] > v2.sparseData[index2]){
                    dist += std::abs( v1.vector[v2.sparseData[index2]] - v2.vector[v2.sparseData[index2]] );
                    index2++; 
                }
                else if (v1.sparseData[index1] == v2.sparseData[index2]){
                    dist += std::abs( v1.vector[v1.sparseData[index1]] - v2.vector[v2.sparseData[index2]] );
                    index1++;
                    index2++;
                }
            }
            return dist;
        }

        // calculate distance with p-norm

        while( (index1 <= sparseElements1 )|| (index2 <= sparseElements2)){
            if ( v1.sparseData[index1] < v2.sparseData[index2]){
                dist += std::pow( v1.vector[v1.sparseData[index1]] - v2.vector[v1.sparseData[index1]], d );
                index1++; 
            }
            else if (v1.sparseData[index1] > v2.sparseData[index2]){
                dist += std::pow( v1.vector[v2.sparseData[index2]] - v2.vector[v2.sparseData[index2]], d );
                index2++; 
            }
            else if (v1.sparseData[index1] == v2.sparseData[index2]){
                dist += std::pow( v1.vector[v1.sparseData[index1]] - v2.vector[v2.sparseData[index2]], d );
                index1++;
                index2++;
            }
        }

        dist = std::pow( dist, 1.0/d );
        return dist;
    }
    else{
        return NumC::dist(v1, v2, d);
    }
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
