// Source: https://stackoverflow.com/questions/8286668/how-to-read-mnist-data-in-c

#include "../include/pandac.h"
#include <fstream>

using namespace std;

int PandaC::reverseInt(int num) {
    unsigned char c1, c2, c3, c4;
    c1 = num & 255;
    c2 = (num >> 8) & 255;
    c3 = (num >> 16) & 255;
    c4 = (num >> 24) & 255;
    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

NumC* PandaC::fromMNIST(char *filePath) {
    ifstream file(filePath, ifstream::in | ifstream::binary);
    if (file.is_open()) {
        printf("file open\n");
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
        file.seekg (sizeof(int), file.beg);
        file.read((char*) &number_of_images, sizeof(int));
        number_of_images= reverseInt(number_of_images);
        file.read((char*) &n_rows, sizeof(int));
        n_rows= reverseInt(n_rows);
        file.read((char*) &n_cols, sizeof(int));
        n_cols= reverseInt(n_cols);
        NumC *data = new NumC(n_rows, n_cols);
        int *row = (int*)malloc(n_cols*sizeof(int));
        for(int i=0;i<number_of_images;++i) {
            for(int r=0;r<n_rows;++r) {
                for(int c=0;c<n_cols;++c) {
                    row[c] = 0;
                    file.read((char*) row+c, sizeof(int));
                }
                data->addVector({row, n_cols});
            }
        }
        cout << "Rows: " << number_of_images << endl;
        cout << "Pictures: " << n_rows << " x " << n_cols << endl;
        // data->print();
        free(row);
        return data;
    }
    perror("Error: PandaC::fromMNIST");
    return NULL;
}