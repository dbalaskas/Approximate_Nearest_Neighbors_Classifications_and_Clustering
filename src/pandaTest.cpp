#include <iostream>
#include <string.h>
#include "../include/pandac.h"

using namespace std;

int main() {
    NumC *dataSet = PandaC::fromMNIST("./doc/input/train-images-idx3-ubyte");
    return 0;
}