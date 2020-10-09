#include <iostream>
#include <string.h>
#include <unistd.h>

#include "../include/pandac.h"
#include "../include/numc.h"
#include "../include/prediction_results.h"
#include "../include/exhaustive_knn.h"
// #include "../include/lsh_classifier.h"

using namespace std;

// Extracts the results from KNN classification compared to Exhaustive Search to output file.
void extractKNNresults(FILE *output, Results results, Results true_results);
// Extracts the results from Range Search to output file.
void extractRSresults(FILE *output, Results results);

// Returns true if the string represents a non-negative number, eitherwise returns false.
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

// Search for <-d> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-d") == 0) break;
	if (i >= argc - 1) {
      	perror("\033[0;31mError!\033[0m Not included '-d' parameter.\n");
        cout << "Executable should be called with: ./" << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *inputFile = argv[i+1];	

// Search for <-q> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-q") == 0) break;
	if (i >= argc - 1) {
      	perror("\033[0;31mError!\033[0m Not included '-q' parameter.\n");
        cout << "Executable should be called with: ./" << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *queryFile = argv[i+1];	

// Search for <-o> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-o") == 0) break;
	if (i >= argc - 1) {
      	perror("\033[0;31mError!\033[0m Not included '-o' parameter.\n");
        cout << "Executable should be called with: ./" << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *outputFile = argv[i+1];

// Search for <-k> parameter.
    int k = 4;
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-k") == 0) break;
	if (i < argc - 1) {
        if (!isNumber(argv[i+1])) {
        // <-k> parameter is invalid.
            perror("\033[0;31mError!\033[0m Invalid value on '-k' parameter.\n");
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        k = atoi(argv[i+1]);
    }

// Search for <-L> parameter.
    int L = 5;
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-L") == 0) break;
	if (i < argc - 1) {
        if (!isNumber(argv[i+1])) {
        // <-L> parameter is invalid.
            perror("\033[0;31mError!\033[0m Invalid value on '-L' parameter.\n");
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        L = atoi(argv[i+1]);
    }

// Search for <-N> parameter.
    int N = 1;
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-L") == 0) break;
	if (i < argc - 1) {
      	if (!isNumber(argv[i+1])) {
        // <-N> parameter is invalid.
            perror("\033[0;31mError!\033[0m Invalid value on '-N' parameter.\n");
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        N = atoi(argv[i+1]);
    }

// Search for <-R> parameter.
    double R = 1.0;
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-L") == 0) break;
	if (i < argc - 1) {
      	if (!isNumber(argv[i+1])) {
        // <-R> parameter is invalid.
            perror("\033[0;31mError!\033[0m Invalid value on '-R' parameter.\n");
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        R = atoi(argv[i+1]);
    }

//------------------------------------------------------------------------------------
// Reading input file.

    // Check that input file exists.
    if(access(inputFile, F_OK) == -1) {
        perror("\033[0;31mError\033[0m: Unable to open the input file");
        cout << "\033[0;31mexit program\033[0m" << endl;
        return 1;
    }
    // Read input file with PandaC.
    NumC* inputData = PandaC::fromMNIST(inputFile);
    NumC::print(inputData->getVector(60000));

//------------------------------------------------------------------------------------
// Making predictions.

char line[128], *answer;
FILE *output;
Results results, true_results;
NumC* queryData;

do {
//------------------------------------------------------------------------------------
// Reading query file.

    // Check that input file exists.
    if(access(queryFile, F_OK) == -1) {
        perror("\033[0;31mError\033[0m: Unable to open the query file");
        cout << "\033[0;31mexit program\033[0m" << endl;
        return 1;
    }
    // Read input file with PandaC.
    queryData = PandaC::fromMNIST(queryFile);
    NumC::print(queryData->getVector(60000));

//------------------------------------------------------------------------------------
// Open output file.

    // Check that input file exists.
    output = fopen(outputFile, "w");
    if (output == NULL) {
        perror("\033[0;31mError\033[0m: Unable to open the output file");
        cout << "\033[0;31mexit program\033[0m" << endl;
        return 1;
    }

// //------------------------------------------------------------------------------------
// // Call LSH classificator and train it.

//     LSH lsh = LSH();
//     lsh.fit_transform(inputData, k, R, L);

// //------------------------------------------------------------------------------------
// // Execute Predictions and extract results to output file.
//     for (i=0; i < queryData->getRows(); i++) {
//     // Execute k-NN prediction.
//         results = lsh.predict_knn(queryData->getVector(i), N);
//     // Execute Exhaustive KNN search.
//         true_results = //...;
//     // Extract results on output file.
//         extractKNNresults(output, results, true_results);
//     // Execute Range Search.
//         results = lsh.predict_rs(queryData->getVector(i), R);
//     // Extract results on output file.
//         extractRSresults(output, results);
//     }

//------------------------------------------------------------------------------------
// Close output file.

    fclose(output);
    output = NULL;

//------------------------------------------------------------------------------------
// Ask user if he wants to repeat the process with new query file.

    cout << endl << "-----------------------------------------------------------------" << endl;
    do {
        cout << "\033[0;36mYou would like to repeat the process with new query? (answer y|n) \033[0m";
		fgets(line,sizeof(line),stdin);
		answer = strtok(line,"\n");
    } while (strcmp(answer, "n") && strcmp(answer, "N") && strcmp(answer, "y") && strcmp(answer, "Y"));
    if (!strcmp(answer, "y") || !strcmp(answer, "Y")) {
    // User wants to repeat the process.
        cout << "\033[0;36mPlease enter a new query file: \033[0m";
		fgets(line,sizeof(line),stdin);
		queryFile = strtok(line,"\n");
        cout << "\033[0;36mPlease enter ann output file (press Enter to use the old one): \033[0m";
        if (strlen(line) > 1) {
    		outputFile = strtok(line,"\n");
        }
        cout << endl;
    }
} while (strcmp(answer, "n") && strcmp(answer, "N"));

//------------------------------------------------------------------------------------
// End of program.

    // cout << endl << "-----------------------------------------------------------------" << endl;
    cout << "\033[0;36mExit program.\033[0m" << endl;

    return 0;
}

void extractKNNresults(FILE *output, Results results, Results true_results) {
}

void extractRSresults(FILE *output, Results results) {
}