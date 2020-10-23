#include <stdio.h>
#include <string.h>

#include <math.h>
#include "../include/hash_function.h"
#include "../include/numc.h"

#include "../include/pandac.h"

#define SIZE_INT 32

using namespace std;

template <typename NumCDataType>
HashFunction<NumCDataType>::HashFunction(int _k, int _dimension, int _w)
: k{_k}, w{_w}, dimension{_dimension}, M{(int)pow(2, (int)(SIZE_INT/this->k))}, m{4586243}, s(k, dimension, false), f_thresholds(k, M, false)
{
    // ============================> this->k = k;
    // ============================> this->w = w;
    // ============================> this->dimension = dimension;

    // this->s = NumC<NumCDataType>(k, dimension);
    // fill with random [0, w)
    this->s.random(w);
    // this->s.print();
    this->f_thresholds.random(2);
    // this->f_thresholds.print();

    // initialize M
    // ============================> this->M = (int)pow(2, (int)(SIZE_INT/this->k));

    // initialize m
    // ============================> this->m = 4586243;
    // this->m = 275;
    // this->m = (int)pow(2, (int)(32 - 32/this->k));
    this->m_d = (int*)malloc(this->dimension*sizeof(int));
    for (int i = 0; i < this->dimension; i++){
        m_d[i] = modularExponentiation(this->m, i, this->M);
    }
    // ============================> this->f_thresholds = initThreasholds(this->k);
}

template <typename NumCDataType>
HashFunction<NumCDataType>& HashFunction<NumCDataType>::operator=(HashFunction<NumCDataType> other_hashFunction) {
    // copy opreations
    this->k = other_hashFunction.k;
    this->w = other_hashFunction.w;
    this->dimension = other_hashFunction.dimension;
    this->s = other_hashFunction.s;

    this->M = (int)pow(2, (int)(SIZE_INT/this->k));
    this->m = 4586243;
    this->m_d = (int*)malloc(this->dimension*sizeof(int));
    memcpy(this->m_d, other_hashFunction.m_d, this->dimension*sizeof(int));
    // ============================> this->f_thresholds = (int*) malloc(k*sizeof(int));
    // ============================> memcpy(this->f_thresholds, other_hashFunction.f_thresholds, k*sizeof(int));
    this->f_thresholds = other_hashFunction.f_thresholds;

    return *this;
}


template <typename NumCDataType>
HashFunction<NumCDataType>::~HashFunction(){
    if (m_d != NULL) {
        free(m_d);
        m_d = NULL;
    }
    // if (f_thresholds != NULL) {
    //     free(f_thresholds);
    //     f_thresholds = NULL;
    // }
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
int* HashFunction<NumCDataType>::initThreasholds(int k){

    // srand(time(NULL));

    // int* thresholds = (int*) malloc(k*sizeof(int));
    // for (int i = 0; i < k; i++){
    //     for (int j = 0; j < M; j++){
    //     thresholds[i] = rand() % this->M;
    //     // cout << thresholds[i] << endl;
    // }

    // return thresholds;
    return 0;
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
    for (int i = 0; i < this->k; i++){
        g  |= (this->h(v, i) << (i* (SIZE_INT/this->k)) );
    }
    

    return g;
}

// template <typename NumCDataType>
// unsigned int HashFunction<NumCDataType>::hc_hash(Vector<NumCDataType> v){
//     unsigned int hash_value = 0;
//     for (int i = 0; i < this->k; i++){
//         if (this->h(v, i) >= f_thresholds[i]) {
//             hash_value = hash_value << 1;
//             hash_value++;
//         } else {
//             hash_value = hash_value << 1;
//         }
//     }
//     return hash_value;
// }

template <typename NumCDataType>
unsigned int HashFunction<NumCDataType>::hc_hash(Vector<NumCDataType> v){
    unsigned int hash_value = 0;
    for (int i = 0; i < this->k; i++){
        hash_value = hash_value << 1;
        hash_value+= (unsigned int) f_thresholds.getElement(i, this->h(v, i));
    }
    return hash_value;
}


// int main(){


//     NumC<int>* inputData = PandaC<int>::fromMNIST("./doc/input/train-images-idx3-ubyte");
//     // // NumC<int>::print(inputData->getVector(0));
//     // // NumC<int>::printSparse(inputData->getVector(1));


//     // NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels("./doc/input/train-labels-idx1-ubyte");
//     // // NumC<int>::print(inputDatalabels->getVector(0));

//     HashFunction<int> hash(4, 28*28,100);

//     // hash.h(inputData->getVector(0), 0);
//     // hash.h(inputData->getVector(0), 1);
//     // hash.h(inputData->getVector(0), 2);
//     // hash.h(inputData->getVector(0), 3);
//     // hash.h(inputData->getVector(0), 4);

//     for (int i = 0; i < 10; i++){
//         cout << "lshHash-> " << hash.lsh_hash(inputData->getVector(i)) <<endl;
//     }
    

// }
