#ifndef NUMC_H
#define NUMC_H

// typedef int VectorDataType;



template <typename NumCDataType>
class Vector{
    public:
        NumCDataType* vector;
        int size;
};

template <typename NumCDataType> 
class NumC {

    private:
        int numOfRows;
        int numOfCols;
        int size;
        NumCDataType* data;

    public:
        NumC();
        NumC(int numOfRows, int numOfCols);
        ~NumC();

        NumC& operator=(NumC other_numc);

        int getRows();
        int getCols();
        NumCDataType* getData();

        void transpose();

        Vector<NumCDataType>  getVector(int index);
        void addElement(NumCDataType element, int row, int col);
        void addVector(Vector<NumCDataType> vector, int index);
        void appendVector(Vector<NumCDataType> vector);

        void print();

        NumC* median();
        static void print(Vector<NumCDataType> vector);
        static double dist(Vector<NumCDataType> v1, Vector<NumCDataType> v2, int d);
};

#endif