#include "./numc.h";
#include "./hc_hashtable.h";

class HyperCube {
    private:
        HC_HashTable** hashTable;
    public:
        HyperCube();
        ~HyperCube();

        void fit(NumC* train_data);
        void transform();
        void fit_transform(NumC* train_data);
        int* predict_knn(Vector vector, double* time);
        int* predict_rs(Vector vector, double* time);
}