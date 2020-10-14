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

    public:
        HashFunction(){};
        HashFunction(int k, int dimension, int w);
        ~HashFunction();
        
        int modularExponentiation(int base, int exponent, int mod);
        int modularAddition(int base, int exponent, int mod);
        int modularMultiplication(int base, int exponent, int mod);

        int h(Vector<NumCDataType> v, int hi);

};





#endif