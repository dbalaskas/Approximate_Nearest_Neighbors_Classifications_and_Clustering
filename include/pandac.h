#ifndef PANDAC_H
#define PANDAC_H

#include <iostream>
#include "./numc.h"

#define NO_LIMIT -1

template <typename NumCDataType>
class PandaC {
    private:
        // Transform Little Endian to Big Endian.
        static int reverseInt(int num);
    public:
        // Creates a NumC object from the MNIST's data.
        static NumC<NumCDataType>* fromMNIST(char* filepath, int limit=NO_LIMIT);
        static NumC<NumCDataType>* fromMNISTlabels(char* filepath, int limit=NO_LIMIT);
};

#endif