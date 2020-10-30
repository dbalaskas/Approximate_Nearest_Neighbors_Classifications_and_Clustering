#ifndef NUMC_H
#define NUMC_H

#include <fstream>
#include <iostream>

typedef int NumCIndexType;
typedef double NumCDistType;

template <typename NumCDataType>
class Vector{
    public:
        NumCDataType* vector;
        NumCIndexType size;
        bool isSparse_;
        NumCIndexType* sparseData;
};

template <typename NumCDataType> 
class NumC {

    private:
        NumCIndexType numOfRows;
        NumCIndexType numOfCols;
        NumCIndexType size;
        NumCDataType* data;
        bool isSparse_;
        NumCIndexType* sparseData;

    public:
        NumC();
        NumC(NumCIndexType numOfRows, NumCIndexType numOfCols, bool isSparse=false);
        ~NumC();

        NumC& operator=(NumC other_numc);


        NumCIndexType getRows();
        NumCIndexType getCols();
        NumCIndexType find(NumCDataType element);
        NumCDataType* getData();
        NumCDataType getLast();
        NumCDataType getElement(NumCIndexType row, NumCIndexType col);

        void transpose();
        void random(NumCDataType maxValue);

        bool isSparse();
        NumCIndexType* getSparseData();

        Vector<NumCDataType>  getVector(NumCIndexType index);
        void addElement(NumCDataType element, NumCIndexType row, NumCIndexType col);
        void addVector(Vector<NumCDataType> vector, NumCIndexType index);
        void addArray(NumC<NumCDataType> array, NumCIndexType index);
        void appendVector(Vector<NumCDataType> vector);
        void fill(NumCDataType fillValue);

        void print(std::ofstream& output=std::cout);

        NumC* median();
        NumCDataType max();
        NumCDataType sum();
        void square();
        void normalize();
        void cumulative();

        static void print(Vector<NumCDataType> vector, std::ofstream& output=std::cout);
        static void printSparse(Vector<NumCDataType> vector, std::ofstream& output=std::cout);
        static NumCDistType dist(Vector<NumCDataType> v1, Vector<NumCDataType> v2, NumCIndexType d);
        static NumCDistType distSparse(Vector<NumCDataType> v1, Vector<NumCDataType> v2, NumCIndexType d);

};

template class NumC<int>;
template class NumC<long>;
template class NumC<double>;

#endif