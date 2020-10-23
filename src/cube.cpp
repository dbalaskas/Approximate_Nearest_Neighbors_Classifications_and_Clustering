#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <fstream>

#include "../include/pandac.h"
#include "../include/numc.h"
#include "../include/prediction_results.h"
#include "../include/exhaustive_knn.h"
#include "../include/hc_classifier.h"

using namespace std;

// Extracts the results to output file.
void extractResults(char* outputFile, Results* results, Results *true_results, vector<Results*> r_results, int R);

// Returns true if the string represents a non-negative number, eitherwise returns false.
bool isNumber(char *word) {
    if (word == NULL) return false;
    // Check each character if it is digit.
    for (int i=0; i<(int) strlen(word); i++)
        if (!isdigit(word[i])) return false;
    return true;
}

// Executable should be called with: 
// ./cube –d <input_file> –q <query_file> -ο <output_file> –k <int> -M <int> -probes <int> -Ν <number_of_nearest> -R <radius>
int main(int argc, char** argv) {

//------------------------------------------------------------------------------------
// Reading inline parameters.
    int i;

// Search for <-d> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-d") == 0) break;
	if (i >= argc - 1) {
      	cout << "\033[0;31mError!\033[0m Not included '-d' parameter.\n" << endl;
        cout << "Executable should be called with: " << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -M <int> -probes <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *inputFile = argv[i+1];	

// Search for <-q> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-q") == 0) break;
	if (i >= argc - 1) {
      	cout << "\033[0;31mError!\033[0m Not included '-q' parameter.\n" << endl;
        cout << "Executable should be called with: " << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -M <int> -probes <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *queryFile = argv[i+1];	

// Search for <-o> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-o") == 0) break;
	if (i >= argc - 1) {
      	cout << "\033[0;31mError!\033[0m Not included '-o' parameter.\n" << endl;
        cout << "Executable should be called with: " << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -M <int> -probes <int> -Ν <number_of_nearest> -R <radius>" << endl << endl;
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
      	    cout << "\033[0;31mError!\033[0m Invalid value on '-k' parameter.\n" << endl;
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        k = atoi(argv[i+1]);
    }

// Search for <-M> parameter.
    int M = 5;
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-M") == 0) break;
	if (i < argc - 1) {
        if (!isNumber(argv[i+1])) {
        // <-M> parameter is invalid.
      	    cout << "\033[0;31mError!\033[0m Invalid value on '-M' parameter.\n" << endl;
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        M = atoi(argv[i+1]);
    }

// Search for <-probes> parameter.
    int probes = 1;
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-probes") == 0) break;
	if (i < argc - 1) {
      	if (!isNumber(argv[i+1])) {
        // <-probes> parameter is invalid.
      	    cout << "\033[0;31mError!\033[0m Invalid value on '-probes' parameter.\n" << endl;
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        probes = atoi(argv[i+1]);
    }

// Search for <-N> parameter.
    int N = 1;
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-N") == 0) break;
	if (i < argc - 1) {
      	if (!isNumber(argv[i+1])) {
        // <-N> parameter is invalid.
      	    cout << "\033[0;31mError!\033[0m Invalid value on '-N' parameter.\n" << endl;
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        N = atoi(argv[i+1]);
    }

// Search for <-R> parameter.
    double R = 1.0;
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-R") == 0) break;
	if (i < argc - 1) {
      	// if (!isNumber(argv[i+1])) {
        // // <-R> parameter is invalid.
      	//     cout << "\033[0;31mError!\033[0m Invalid value on '-R' parameter.\n" << endl;
        //     cout << "\033[0;31mExit program.\033[0m" << endl;
        //     return 1;        
        // }
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
    cout << "\033[0;36mRunning Hypercube :)\033[0m" << endl << endl;
    // Read input file with PandaC.
    NumC<int>* inputData = PandaC<int>::fromMNIST(inputFile);

//------------------------------------------------------------------------------------
// Reading query file.

    // Check that query file exists.
    if(access(queryFile, F_OK) == -1) {
        perror("\033[0;31mError\033[0m: Unable to open the query file");
        cout << "\033[0;31mexit program\033[0m" << endl;
        return 1;
    }
    // Read query file with PandaC.
    NumC<int> *queryData = PandaC<int>::fromMNIST(queryFile, 50);

//------------------------------------------------------------------------------------
// Making predictions.

char line[128], *answer;
Results *knn_results, *true_results;
vector<Results*> r_results;

do {
    cout << "\033[0;36mComputing predictions...\033[0m" << endl << endl;
//------------------------------------------------------------------------------------
// Call HyperCube classifier and train it.

    HyperCube<int> cube(50000);
    cube.fit_transform(inputData, k);

//------------------------------------------------------------------------------------
// Call exhaustive knn classifier and train it.

    ExhaustiveKnn<int> exhaustive_knn(inputData, N);
//------------------------------------------------------------------------------------
// Execute Predictions and extract results to output file.

    // Execute k-NN prediction.
    knn_results = cube.predict_knn(queryData, N, M, probes);
    // Execute Exhaustive KNN search.
    true_results = exhaustive_knn.predict_knn(queryData);
    // Execute Range Search.
    r_results = cube.predict_rs(queryData, R, M, probes);
    // Extract results on output file.
    extractResults(outputFile, knn_results, true_results, r_results, R);
    cout << "\033[0;36mResults are extracted in file: \033[0m" << outputFile << endl;

//------------------------------------------------------------------------------------
// Free allocated Space.

    delete queryData;
    delete knn_results;
    delete true_results;
    for (int i=0; i < (int) r_results.size(); i++) {
        delete r_results[i];
    }

//------------------------------------------------------------------------------------
// Ask user if he wants to repeat the process with new query file.

    cout << "-----------------------------------------------------------------" << endl;
    do {
        cout << "\033[0;36mYou would like to repeat the process with new query? (answer y|n) \033[0m";
		fgets(line,sizeof(line),stdin);
		answer = strtok(line,"\n");
    } while (strcmp(answer, "n") && strcmp(answer, "N") && strcmp(answer, "y") && strcmp(answer, "Y"));
    if (!strcmp(answer, "y") || !strcmp(answer, "Y")) {
    // User wants to repeat the process.
        cout << "\033[0;36mPlease enter a new query file: \033[0m";
		fgets(line,sizeof(line),stdin);
        if (strlen(line) > 1) {
            queryFile = strtok(line,"\n");
            // Check that query file exists.
            if(access(queryFile, F_OK) == -1) {
                perror("\033[0;31mError\033[0m: Unable to open the query file");
                cout << "\033[0;31mexit program\033[0m" << endl;
                return 1;
            }
            // Read query file with PandaC.
            queryData = PandaC<int>::fromMNIST(queryFile, 50);
        } else {
            printf("\033[0;31mError\033[0m: Not imported query file");
            cout << "\033[0;31mexit program\033[0m" << endl;
        }
        cout << "\033[0;36mPlease enter ann output file (press Enter to use the old one): \033[0m";
		fgets(line,sizeof(line),stdin);
        cout << endl;
        if (strlen(line) > 1) {
    		outputFile = strtok(line,"\n");
        }
    }    
} while (strcmp(answer, "n") && strcmp(answer, "N"));

//------------------------------------------------------------------------------------
// End of program.

    delete inputData;

    // cout << endl << "-----------------------------------------------------------------" << endl;
    cout << "\033[0;36mExit program.\033[0m" << endl;
    return 0;
}

void extractResults(char* outputFile, Results* results, Results *true_results, vector<Results*> r_results, int R) {
    NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels((char*) "./doc/input/train-labels-idx1-ubyte");

    ofstream output(outputFile, ios::out);
    // Check that output file exists.
    if (!output.is_open()) {
        perror("\033[0;31mError\033[0m: Unable to open output file");
        return;
    }

    for (int i=0; i < results->resultsIndexArray.getRows(); i++) {
        output << "Query: " << i+1 << endl;
        for (int j=0; j < results->resultsIndexArray.getCols(); j++) {
            // output << "  Nearest neighbor-" << j+1 << ": " << results->resultsIndexArray.getElement(i, j) << endl;
            output << "  Nearest neighbor-" << j+1 << ": " << results->resultsIndexArray.getElement(i, j) << " label: " << inputDatalabels->getElement(results->resultsIndexArray.getElement(i, j) ,0) << " true label: " << inputDatalabels->getElement(true_results->resultsIndexArray.getElement(i, j) ,0)<<endl;
            output << "  distanceHypecube: " << results->resultsDistArray.getElement(i, j) << endl;
            output << "  distanceTrue: " << true_results->resultsDistArray.getElement(i, j) << endl;
        }
        output << "  tHypercube: " << results->executionTimeArray.getElement(i, 0) << endl;
        output << "  tTrue: " << true_results->executionTimeArray.getElement(i, 0) << endl;
        output << "  " << R << "-near neighbors:" << endl;
        for (int j=0; j < r_results[i]->resultsIndexArray.getCols(); j++) {
            // output << "    " << r_results[i]->resultsIndexArray.getElement(0, j) << endl;
            output << "    " << r_results[i]->resultsIndexArray.getElement(0, j) << " label: " << inputDatalabels->getElement(r_results[i]->resultsIndexArray.getElement(0, j) ,0)<<endl;
        }
    }

    // Close output file.
    output.close();
    delete inputDatalabels;
}