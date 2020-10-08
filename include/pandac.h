#ifndef PANDAC_H
#define PANDAC_H

#include <iostream>
#include "./numc.h"

class PandaC {
    private:
        static int reverseInt(int num);
    public:
        static void toDat(FILE* filepath, char* text);
        static NumC* fromMNIST(char* filepath);
};

#endif