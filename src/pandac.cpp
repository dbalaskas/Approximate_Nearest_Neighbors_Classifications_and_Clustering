// Source: https://stackoverflow.com/questions/8286668/how-to-read-mnist-data-in-c
//         https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte

#include "../include/pandac.h"
#include <fstream>
#include <string.h>

using namespace std;

template class PandaC<int>;
template class PandaC<long>;
template class PandaC<double>;

template <typename NumCDataType>
int PandaC<NumCDataType>::reverseInt(int num) {
    unsigned char c1, c2, c3, c4;
    c1 = num & 255;
    c2 = (num >> 8) & 255;
    c3 = (num >> 16) & 255;
    c4 = (num >> 24) & 255;
    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

unsigned char reverseChar(unsigned char num) {
   num = (num & 0xF0) >> 4 | (num & 0x0F) << 4;
   num = (num & 0xCC) >> 2 | (num & 0x33) << 2;
   num = (num & 0xAA) >> 1 | (num & 0x55) << 1;
   return num;
}

template <typename NumCDataType>
NumC<NumCDataType>* PandaC<NumCDataType>::fromMNIST(char *filePath) {
    ifstream file(filePath, ifstream::in | ifstream::binary);
    if (file.is_open()) {
        printf("Open file\n");
        int number_of_images = 0;
        int n_cols_of_matrix = 0;
        int n_rows_of_image  = 0;
        int n_cols_of_image  = 0;
        
    // Ignore magic number.
        file.seekg(sizeof(int), file.beg);

    // Read the metadata.
        file.read((char*) &number_of_images, sizeof(int));
        number_of_images= reverseInt(number_of_images);
        file.read((char*) &n_rows_of_image, sizeof(int));
        n_rows_of_image = reverseInt(n_rows_of_image);
        file.read((char*) &n_cols_of_image, sizeof(int));
        n_cols_of_image = reverseInt(n_cols_of_image);

    // initialize martix to store all the images' data.
        n_cols_of_matrix = n_cols_of_image*n_rows_of_image;
        NumC<NumCDataType> *data = new NumC<NumCDataType>(number_of_images, n_cols_of_image*n_rows_of_image, true);

        // read the pixels of every image
        char *pixel = (char*)malloc(n_cols_of_matrix*sizeof(char));
        NumCDataType pixelType;

    // read the pixels of every image.
        char *image = (char*)malloc(n_cols_of_matrix*sizeof(char));
        int pixel;
        for(int i=0;i<number_of_images; ++i) {

            // read all the pixels of an image
            file.read( pixel, sizeof(char)*n_cols_of_matrix);
            for( int j = 0; j < n_cols_of_matrix; j++){
                // reverse the char pixel and store in int
                pixelType =  (NumCDataType)reverseChar(pixel[j]);
                data->addElement(pixelType, i, j);
            }
        }

    // Print file's info.
        cout << "Rows: " << number_of_images << endl;
        cout << "Pictures: " << n_rows_of_image << " x " << n_cols_of_image << endl;
        // data->print();

    // Free allocated space and return data's matrix.
        free(image);
        return data;
    }

// Unable to open file, return NULL.
    perror("Error: PandaC::fromMNIST");
    return NULL;
}

template <typename NumCDataType>
NumC<NumCDataType>* PandaC<NumCDataType>::fromMNISTlabels(char *filePath) {
    ifstream file(filePath, ifstream::in | ifstream::binary);
    if (file.is_open()) {
        printf("Open file\n");
        int number_of_images = 0;
        int n_cols_of_matrix = 1;
        
    // Ignore magic number.
        // file.read((char*) &number_of_images, sizeof(int));
        file.seekg (sizeof(int), file.beg);

    // Read the metadata.
        file.read((char*) &number_of_images, sizeof(int));
        number_of_images= reverseInt(number_of_images);


        // initialize martix to store all the labels
        NumC<NumCDataType> *data = new NumC<NumCDataType>(number_of_images, n_cols_of_matrix, true);

    // read the pixels of every image
        char *label = (char*)malloc(n_cols_of_matrix*sizeof(char));
        NumCDataType labelType;

        for(int i=0;i<number_of_images; ++i) {

            // read all the pixels of an image
            file.read( label, sizeof(char)*n_cols_of_matrix);
            for( int j = 0; j < n_cols_of_matrix; j++){
                // do not reverse the char pixel and store in int
                labelType =  (NumCDataType)label[j];
                data->addElement(labelType, i, j);
            }
        }
        
    // Print file's info.
        cout << "Rows: " << number_of_images << endl;
        
    // Free allocated space and return labels's matrix.
        free(label);
        return data;
    }

// Unable to open file, return NULL.
    perror("Error: PandaC::fromMNIST");
    return NULL;
}