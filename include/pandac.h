#ifndef PANDAC_H
#define PANDAC_H

#include <iostream>
#include "./numc.h"

// Class for reading the files
class PandaC {
    private:
        // Transform Little Endian to Big Endian.
        static int reverseInt(int num);
    public:
        // Creates a NumC object from the MNIST's data file.
        static NumC* fromMNIST(char* filepath);
        // Creates a NumC object from the MNIST's labels file.
        static NumC* fromMNISTlabels(char* filepath);
};

#endif