#include <math.h>
#include "../include/hash_function.h"
#include "../include/numc.h"

#include "../include/pandac.h"

using namespace std;

template <typename NumCDataType>
HashFunction<NumCDataType>::HashFunction(int k, int dimension, int w){
    this->k = k;
    this->w = w;
    this->dimension = dimension;
    this->s = NumC<NumCDataType>(k, dimension);
    // fill with random [0, w)
    this->s.random(w);
    // this->s.print();

    // initialize M
    this->M = (int)pow(2, (int)(32/this->k));

    // initialize m
    this->m = 4586243;
    // this->m = (int)pow(2, (int)(32 - 32/this->k));
    this->m_d = (int*)malloc(this->dimension*sizeof(int));
    for (int i = 0; i < this->dimension; i++){
        m_d[i] = modularExponentiation(this->m, i, this->M);
        // cout<<m_d[i]<<endl;
    }
    
}

template <typename NumCDataType>
HashFunction<NumCDataType>::~HashFunction(){
    free(m_d);
}

template <typename NumCDataType>
int HashFunction<NumCDataType>::modularExponentiation(int base, int exponent, int mod){

    if (mod == 1)
        return 0;

    unsigned int c = 1;
    for (int i = 0; i < exponent; i++){
        c = (c * (unsigned int)base) % mod;
        // cout<<c<<endl;
    }
    
    return (int)c;
}

template <typename NumCDataType>
int HashFunction<NumCDataType>::modularAddition(int base, int exponent, int mod){

    if (mod == 1)
        return 0;

    int c = 1;
    for (int i = 0; i < exponent; i++){
        c = (c * base) % mod;
    }
    
    return c;
}

template <typename NumCDataType>
int HashFunction<NumCDataType>::modularMultiplication(int base, int exponent, int mod){

    if (mod == 1)
        return 0;

    int c = 1;
    for (int i = 0; i < exponent; i++){
        c = (c * base) % mod;
    // cout<<c<<endl;
    }
    
    return c;
}

template <typename NumCDataType>
int HashFunction<NumCDataType>::h(Vector<NumCDataType> v, int hi){

    // calculate a
    int* a = (int*)malloc(this->dimension * sizeof(int));
    for (int i = 0; i < this->dimension; i++){
        a[i] = (int)floor( (double)( v.vector[i] - s.getElement(hi, i) ) / (double)w );
        // cout << a[i] << " ,";
    }
    
    // calculate sums of a
    // cout << modularExponentiation(4, 13, 497) <<endl;
    int sum_h = 0;
    int h = 0;
    for (int i = 0; i < this->dimension; i++){
        sum_h += ( abs(a[this->dimension -1 -i] % this->M)  *   this->m_d[i]) % this->M;
    }
    h = sum_h % this->M;
    // cout <<"HHH "<<h<<endl;
    
    free(a);

    return h;
}

template <typename NumCDataType>
unsigned int HashFunction<NumCDataType>::lsh_hash(Vector<NumCDataType> v){

    // initialize g
    unsigned int g = 0;

    // concat hi to g
    for (int i = 0; i < (8*sizeof(int))/this->k; i++){
        g  |= (this->h(v, i) << (i*this->k));
    }
    

    return g;
}

template <typename NumCDataType>
unsigned int HashFunction<NumCDataType>::hc_hash(Vector<NumCDataType> v){

    ;

}


// int main(){


//     NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
//     // // NumC<int>::print(inputData->getVector(0));
//     // // NumC<int>::printSparse(inputData->getVector(1));


//     // NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");
//     // // NumC<int>::print(inputDatalabels->getVector(0));

//     HashFunction<int> hash(5, 28*28,100);

//     // hash.h(inputData->getVector(0), 0);
//     // hash.h(inputData->getVector(0), 1);
//     // hash.h(inputData->getVector(0), 2);
//     // hash.h(inputData->getVector(0), 3);
//     // hash.h(inputData->getVector(0), 4);

//     for (int i = 0; i < 10; i++){
//         cout << "lshHash-> " << hash.lsh_hash(inputData->getVector(i)) <<endl;
//     }
    

// }