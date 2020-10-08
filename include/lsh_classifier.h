#include "./numc.h";
#include "./lsh_hashtable.h";

class LSH {
    private:
        LSH_HashTable** hashTable;
    public:
        LSH();
        ~LSH();

        void fit(NumC* train_data);
        void transform();
        void fit_transform(NumC* train_data);
        int* predict_knn(Vector vector, double* time);
        int* predict_rs(Vector vector, double* time);
}