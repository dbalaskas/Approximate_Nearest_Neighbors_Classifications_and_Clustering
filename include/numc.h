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
        int size;
        VectorDataType* data;

    public:
        NumC(int numOfRows, int numOfCols);
        ~NumC();

        int getRows();
        int getCols();

        Vector getVector(int index);
        void addElement(VectorDataType element, int row, int col);
        void addVector(Vector vector);

        void print();

        NumC* median();
        static void print(Vector vector);
        static double dist(Vector v1, Vector v2, int d);
};

#endif