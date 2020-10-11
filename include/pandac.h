#ifndef PANDAC_H
#define PANDAC_H

#include <iostream>
#include "./numc.h"

class PandaC {
    private:
        // Transform Little Endian to Big Endian.
        static int reverseInt(int num);
    public:
        // Creates a NumC object from the MNIST's data.
        static NumC* fromMNIST(char* filepath);
        static NumC* fromMNISTlabels(char* filepath);
};

#endif