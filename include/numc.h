#ifndef NUMC_H
#define NUMC_H

typedef int VectorDataType;
typedef struct {
    VectorDataType* vector;
    int size;
} Vector;

class NumC {

    private:
        int numOfRows;
        int numOfCols;
        VectorDataType* data;

    public:
        NumC(int numOfRows, int numOfCols);
        ~NumC();

        int getRows();
        int getCols();

        Vector getVector(int index);
        void addElement(VectorDataType element, int row, int col);
        void addVector(int *vector);

        void print();

        NumC* median();
        static double dist(VectorDataType* v1, VectorDataType* v2, int d);
};


#endif