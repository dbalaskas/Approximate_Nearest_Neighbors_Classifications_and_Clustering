#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <fstream>

#include "../include/pandac.h"
#include "../include/numc.h"
#include "../include/prediction_results.h"
#include "../include/exhaustive_knn.h"
#include "../include/lsh_classifier.h"

using namespace std;

// Extracts the results to output file.
bool extractResults(char* outputFile, Results* results, Results *true_results, vector<Results*> r_results, int R);

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
      	cout << "\033[0;31mError!\033[0m Not included '-d' parameter." << endl;
        cout << "Executable should be called with: " << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *inputFile = argv[i+1];	

// Search for <-q> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-q") == 0) break;
	if (i >= argc - 1) {
      	cout << "\033[0;31mError!\033[0m Not included '-q' parameter." << endl;
        cout << "Executable should be called with: " << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *queryFile = argv[i+1];	

// Search for <-o> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-o") == 0) break;
	if (i >= argc - 1) {
      	cout << "\033[0;31mError!\033[0m Not included '-o' parameter." << endl;
        cout << "Executable should be called with: " << argv[0] << " –d <input_file> –q <query_file> -ο <output_file> –k <int> -L <int> -Ν <number_of_nearest> -R <radius>" << endl;
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
      	    cout << "\033[0;31mError!\033[0m Invalid value on '-k' parameter." << endl;
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
      	    cout << "\033[0;31mError!\033[0m Invalid value on '-L' parameter." << endl;
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        L = atoi(argv[i+1]);
    }

// Search for <-N> parameter.
    int N = 1;
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-N") == 0) break;
	if (i < argc - 1) {
      	if (!isNumber(argv[i+1])) {
        // <-N> parameter is invalid.
      	    cout << "\033[0;31mError!\033[0m Invalid value on '-N' parameter." << endl;
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
      	if (!isNumber(argv[i+1])) {
        // <-R> parameter is invalid.
      	    cout << "\033[0;31mError!\033[0m Invalid value on '-R' parameter." << endl;
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;        
        }
        R = atoi(argv[i+1]);
    }

//------------------------------------------------------------------------------------
// Reading input file.

    // Check that input file exists.
    if(access(inputFile, F_OK) == -1) {
        perror("\033[0;31mError\033[0m: Unable to open input file");
        cout << "\033[0;31mexit program\033[0m" << endl;
        return 1;
    }
    cout << "\033[0;36mRunning LSH :)\033[0m" << endl << endl;
    // Read input file with PandaC.
    NumC<int>* inputData = PandaC<int>::fromMNIST(inputFile);

//------------------------------------------------------------------------------------
// Reading query file.

    // Check that input file exists.
    if(access(queryFile, F_OK) == -1) {
        perror("\033[0;31mError\033[0m: Unable to open query file");
        cout << "\033[0;31mexit program\033[0m" << endl;
        return 1;
    }
    // Read query file with PandaC.
    NumC<int>* queryData = PandaC<int>::fromMNIST(queryFile, 50);

//------------------------------------------------------------------------------------
// Making predictions.

char line[128], *answer;
Results *knn_results, *true_results;
vector<Results*> r_results;

    do {
        cout << "\033[0;36mComputing predictions...\033[0m" << endl << endl;
    //------------------------------------------------------------------------------------
    // Call LSHashing classifier and train it.

        LSHashing<int> lsh(L, k, 50000);
        lsh.fit_transform(inputData);

    //------------------------------------------------------------------------------------
    // Call exhaustive knn classifier and train it.

        ExhaustiveKnn<int> exhaustive_knn(inputData, N);

    //------------------------------------------------------------------------------------
    // Execute Predictions and extract results to output file.

        // Execute k-NN prediction.
        knn_results = lsh.predict_knn(queryData, N);
        // Execute Exhaustive KNN search.
        true_results = exhaustive_knn.predict_knn(queryData);
        // Execute Range Search.
        r_results = lsh.predict_rs(queryData, R);
        // Extract results on output file.
        if (extractResults(outputFile, knn_results, true_results, r_results, R)) {
            cout << "Results are extracted in file: " << outputFile << endl;
        }

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
            cout << "\033[0;36mPlease enter a new query file (press Enter to use the old one): \033[0m";
    		fgets(line,sizeof(line),stdin);
            if (strlen(line) > 1) {
                queryFile = strtok(line,"\n");
                // Check that query file exists.
                if(access(queryFile, F_OK) == -1) {
                    perror("\033[0;31mError\033[0m: Unable to open query file");
                    cout << "\033[0;31mexit program\033[0m" << endl;
                    return 1;
                }
                // Read query file with PandaC.
                queryData = PandaC<int>::fromMNIST(queryFile, 50);
            } else {
                if(access(queryFile, F_OK) == -1) {
                    perror("\033[0;31mError\033[0m: Unable to open query file");
                    cout << "\033[0;31mexit program\033[0m" << endl;
                    return 1;
                }
                // Read query file with PandaC.
                queryData = PandaC<int>::fromMNIST(queryFile, 50);
            }
            cout << "\033[0;36mPlease enter an output file (press Enter to use the old one): \033[0m";
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

bool extractResults(char* outputFile, Results* results, Results *true_results, vector<Results*> r_results, int R) {
    // NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels((char*) "./doc/input/train-labels-idx1-ubyte");

    // Check that output file exists.
    ofstream output(outputFile, fstream::out);
    if (!output.is_open()) {
        perror("\033[0;31mError\033[0m: Unable to open output file");
        return false;
    }

    double sumDistanceLSH = 0;
    double sumTimeLSH = 0;
    double sumDistanceTrue = 0;
    double sumTimeTrue = 0;
    for (int i=0; i < results->resultsIndexArray.getRows(); i++) {
        output << "Query: " << i+1 << endl;
        for (int j=0; j < results->resultsIndexArray.getCols(); j++) {
            output << "  Nearest neighbor-" << j+1 << ": " << results->resultsIndexArray.getElement(i, j) << endl;
            // output << "  Nearest neighbor-" << j+1 << ": " << results->resultsIndexArray.getElement(i, j) << " label: " << inputDatalabels->getElement(results->resultsIndexArray.getElement(i, j) ,0) << " true label: " << inputDatalabels->getElement(true_results->resultsIndexArray.getElement(i, j) ,0)<<endl;
            output << "   distanceLSH:  " << results->resultsDistArray.getElement(i, j) << endl;
            sumDistanceLSH += results->resultsDistArray.getElement(i, j);
            output << "   distanceTrue: " << true_results->resultsDistArray.getElement(i, j) << endl;
            sumDistanceTrue += true_results->resultsDistArray.getElement(i, j);
        }
        output << "  tLSH: " << results->executionTimeArray.getElement(i, 0) << endl;
        sumTimeLSH += results->executionTimeArray.getElement(i, 0);
        output << "  tTrue: " << true_results->executionTimeArray.getElement(i, 0) << endl;
        sumTimeTrue += true_results->executionTimeArray.getElement(i, 0);
        output << "  " << R << "-near neighbors:" << endl;
        for (int j=0; j < r_results[i]->resultsIndexArray.getCols(); j++) {
            output << "    " << r_results[i]->resultsIndexArray.getElement(0, j) << endl;
            // output << "    " << r_results[i]->resultsIndexArray.getElement(0, j) << " label: " << inputDatalabels->getElement(r_results[i]->resultsIndexArray.getElement(0, j) ,0)<<endl;
        }
        output << endl;
    }

    // Printing Score of prediction.
    cout << "Final Exhaustive vs LSH Score:" << endl;
    cout << "Average LSH distance: " << (double) sumDistanceLSH/(results->resultsIndexArray.getCols() * results->resultsIndexArray.getRows()) << endl;
    cout << "Average True distance: " << (double) sumDistanceTrue/(results->resultsIndexArray.getCols() * results->resultsIndexArray.getRows()) << endl;
    cout << "LSH/True distance: " << (double) sumDistanceLSH/sumDistanceTrue << " (LSH has better accuracy when this number is inclined to 1)" << endl;
    cout << "LSH's fault is " << (int) ((((double) sumDistanceLSH/sumDistanceTrue)-1)*100) << "% on average prediction." << endl;
    cout << endl;
    cout << "Average LSH Time: " << (double) sumTimeLSH/results->resultsIndexArray.getRows() << endl;
    cout << "Average True Time: " << (double) sumTimeTrue/results->resultsIndexArray.getRows() << endl;
    cout << "LSH/True Time: " << (double) sumTimeLSH/sumTimeTrue << " (LSH is faster when this number is inclined to 0)" << endl;
    if (sumTimeLSH > sumTimeTrue) {
        cout << "LSH's time is " << (int) ((((double) sumTimeLSH/sumTimeTrue)-1)*100) << "% slower than true." << endl;
    } else {
        cout << "LSH's time is " << (int) ((1-((double) sumTimeLSH/sumTimeTrue))*100) << "% faster than true." << endl;
    }
    cout << endl;

    // Close output file.
    output.close();
    // delete inputDatalabels;

    return true;
}