#include <iostream>
#include <string.h>

using namespace std;


bool isNumber(char *word) {
    if (word == NULL) return false;
    // Check each character if it is digit.
    for (int i=0; i<(int) strlen(word); i++)
        if (!isdigit(word[i])) return false;
    return true;
}

// Executable should be called with: 
// ./lsh –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>
int main(int argc, char** argv) {

//------------------------------------------------------------------------------------
// Reading inline parameters.
    int i;

// Search for -i parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-i") == 0) break;
	if (i >= argc - 1) {
      	fprintf(stderr, "\033[0;31mError!\033[0m Not included '-i' parameter.\n");
        cout << "Executable should be called with: ./" << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *inputFile = argv[i+1];	

// Search for -q parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-q") == 0) break;
	if (i >= argc - 1) {
      	fprintf(stderr, "\033[0;31mError!\033[0m Not included '-q' parameter.\n");
        cout << "Executable should be called with: ./" << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *queryFile = argv[i+1];	

// Search for -o parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-o") == 0) break;
	if (i >= argc - 1) {
      	fprintf(stderr, "\033[0;31mError!\033[0m Not included '-o' parameter.\n");
        cout << "Executable should be called with: ./" << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *outputFile = argv[i+1];

// Search for -k parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-k") == 0) break;
	if (i >= argc - 1) {
      	fprintf(stderr, "\033[0;31mError!\033[0m Not included '-k' parameter.\n");
        cout << "Executable should be called with: ./" << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
    if (!isNumber(argv[i+1])) {
	// If the -k parameter is invalid.
      	fprintf(stderr, "\033[0;31mError!\033[0m Invalid value on '-k' parameter.\n");
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;        
    }
    int k = atoi(argv[i+1]);

// Search for -L parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-L") == 0) break;
	if (i >= argc - 1) {
      	fprintf(stderr, "\033[0;31mError!\033[0m Not included '-L' parameter.\n");
        cout << "Executable should be called with: ./" << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	} else if (!isNumber(argv[i+1])) {
    // If the -L parameter is invalid.
        fprintf(stderr, "\033[0;31mError!\033[0m Invalid value on '-L' parameter.\n");
        cout << "\033[0;31mExit program.\033[0m" << endl;
        return 1;        
    } else {
        int L = atoi(argv[i+1]);
    }

// Search for -N parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-L") == 0) break;
	if (i >= argc - 1) {
      	fprintf(stderr, "\033[0;31mError!\033[0m Not included '-N' parameter.\n");
        cout << "Executable should be called with: ./" << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	} else if (!isNumber(argv[i+1])) {
    // If the -N parameter is invalid.
        fprintf(stderr, "\033[0;31mError!\033[0m Invalid value on '-N' parameter.\n");
        cout << "\033[0;31mExit program.\033[0m" << endl;
        return 1;        
    } else {
        int N = atoi(argv[i+1]);
    }

// Search for -R parameter.
    double R = 1.0;
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-L") == 0) break;
	if (i < argc - 1) {
      	if (!isNumber(argv[i+1])) {
        // If the -R parameter is invalid.
            fprintf(stderr, "\033[0;31mError!\033[0m Invalid value on '-R' parameter.\n");
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        R = atoi(argv[i+1]);
    }

//------------------------------------------------------------------------------------
// Third: Checking if thenumofupdated has been called ("-n")
	unsigned int numofUpdates = 5;	
	for (int i= 1; i<argc; i++) {
			if (strcmp(argv[i], "-n") == 0) {
					numofUpdates = atoi(argv[++i]);
					break;
			}
	}

    return 0;
}