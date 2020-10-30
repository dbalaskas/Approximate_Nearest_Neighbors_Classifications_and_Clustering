#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H


#include "./numc.h"

template <typename NumCDataType>
class HashFunction{

    private:
        int k;
        int w;
        int dimension;
        int M;
        int m;
        int* m_d;
        NumC<NumCDataType> s;
        NumC<NumCDataType> f_thresholds;

        // Definition of modular on varius mathematical operations.
        int modularExponentiation(int base, int exponent, int mod);
        int modularAddition(int base, int exponent, int mod);
        int modularMultiplication(int base, int exponent, int mod);

    public:
        HashFunction(): m_d{NULL} {};
        HashFunction(int k, int dimension, int w);
        HashFunction<NumCDataType>& operator=(HashFunction<NumCDataType> other_hashFunction);
        ~HashFunction();

        int getk() {return k;};
        int getw() {return w;};
        int getM() {return M;};

        // Returns the 'h' value of 'v'.
        int h(Vector<NumCDataType> v, int hi);

        // Returns the LSH hash value of 'v'.
        unsigned int lsh_hash(Vector<NumCDataType> v);
        // Returns the Hypercube hash value of 'v'.
        unsigned int hc_hash(Vector<NumCDataType> v);

};

// Define the templates of Hypercube
template class HashFunction<int>;
template class HashFunction<long>;
template class HashFunction<double>;

#endif