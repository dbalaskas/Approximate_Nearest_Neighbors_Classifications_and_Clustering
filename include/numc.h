

typedef int* VectorType;

class NumC {
    int numOfRows;
    int numOfCols;
    VectorType data;

    private:

    public:
        NumC();
        ~NumC();
        int getRows();
        int getCols();
        VectorType getVector(int index);
        void addVector(int *vector);
        NumC* median();

        static double dist(VectorType v1, VectorType v2, int d);
};