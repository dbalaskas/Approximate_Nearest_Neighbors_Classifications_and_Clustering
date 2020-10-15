#ifndef NUMC_H
#define NUMC_H

typedef int NumCIndexType;
typedef double NumCDistType;

template <typename NumCDataType>
class Vector{
    public:
        NumCDataType* vector;
        NumCIndexType size;
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
        NumCDataType* getData();
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

        void print();

        NumC* median();
        static void print(Vector<NumCDataType> vector);
        static void printSparse(Vector<NumCDataType> vector);
        static NumCDistType dist(Vector<NumCDataType> v1, Vector<NumCDataType> v2, NumCIndexType d);
        static NumCDistType distSparse(Vector<NumCDataType> v1, Vector<NumCDataType> v2, NumCIndexType d);

};

template class NumC<int>;
template class NumC<long>;
template class NumC<double>;

#endif