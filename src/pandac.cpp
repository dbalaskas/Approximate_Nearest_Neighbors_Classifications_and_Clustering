#include "../include/pandac.h"
#include <fstream>

using namespace std;

// void PandaC::toDat(FILE* filepath, char* text) {

// }

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
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
        file.read((char*) &magic_number, sizeof(int)); 
        magic_number= reverseInt(magic_number);
        file.read((char*) &number_of_images, sizeof(int));
        number_of_images= reverseInt(number_of_images);
        file.read((char*) &n_rows, sizeof(int));
        n_rows= reverseInt(n_rows);
        file.read((char*) &n_cols, sizeof(int));
        n_cols= reverseInt(n_cols);
        cout << "Read " << number_of_images << endl;
        cout << "Read " << n_rows << endl;
        cout << "Read " << n_cols << endl;
        NumC *data = new NumC(n_rows, n_cols);
        int row[n_rows]; 
        for(int i=0;i<number_of_images;++i) {
            for(int r=0;r<n_rows;++r) {
                for(int c=0;c<n_cols;++c) {
                    // unsigned char temp=0;
                    // file.read((char*) &temp,sizeof(temp));
                    row[c] = 0;
                    file.read((char*) row+c, sizeof(int));
                }
                data->addVector(row);
            }
        }
        data->print();
        return data;
    }
    printf("error\n");
    return NULL;
}