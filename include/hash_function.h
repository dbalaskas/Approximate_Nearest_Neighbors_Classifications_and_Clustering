#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H


#include "./numc.h"
#include <iostream>

template <typename NumCDataType>
class HashFunction{

    private:
        int k;
        int w;
        int dimension;
        int M;
        int m;
        int* m_d;
        int* f_thresholds;
        NumC<NumCDataType> s;
        int modularExponentiation(int base, int exponent, int mod);
        int modularAddition(int base, int exponent, int mod);
        int modularMultiplication(int base, int exponent, int mod);
        int* initThreasholds(int k);

    public:
        HashFunction(): m_d{NULL}, f_thresholds{NULL} {};
        HashFunction(int k, int dimension, int w);
        ~HashFunction();
        

        int h(Vector<NumCDataType> v, int hi);

        unsigned int lsh_hash(Vector<NumCDataType> v);
        unsigned int hc_hash(Vector<NumCDataType> v);

};

template class HashFunction<int>;
template class HashFunction<long>;
template class HashFunction<double>;

#endif