#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <fstream>

#include "../include/pandac.h"
#include "../include/numc.h"
#include "../include/prediction_results.h"
#include "../include/kmedians.h"
#include "../include/exhaustive_knn.h"
#include "../include/lsh_classifier.h"

using namespace std;

// Extracts the results to output file.
bool extractResults(char* outputFile, char* method, bool complete, Kmedians<int> *kMedians);

// Returns true if the string represents a non-negative number, eitherwise returns false.
bool isNumber(char *word) {
    if (word == NULL) return false;
    // Check each character if it is digit.
    for (int i=0; i<(int) strlen(word); i++)
        if (!isdigit(word[i])) return false;
    return true;
}

// Executable should be called with: 
// ./cluster –i <input_file> –c <configuration_file> -o <output_file> -m <method: 'Classic' OR 'LSH' OR 'Hypercube'> -complete (optional)
int main(int argc, char** argv) {

//------------------------------------------------------------------------------------
// Reading inline parameters.
    int i;

// Search for <-i> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-i") == 0) break;
	if (i >= argc - 1) {
      	cout << "\033[0;31mError!\033[0m Not included '-i' parameter." << endl;
        cout << "Executable should be called with: " << argv[0] << " –i <input_file> –c <configuration_file> -o <output_file> -m <method: 'Classic' OR 'LSH' OR 'Hypercube'> -complete (optional)" << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *inputFile = argv[i+1];	

// Search for <-c> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-c") == 0) break;
	if (i >= argc - 1) {
      	cout << "\033[0;31mError!\033[0m Not included '-c' parameter." << endl;
        cout << "Executable should be called with: " << argv[0] << " –i <input_file> –c <configuration_file> -o <output_file> -m <method: 'Classic' OR 'LSH' OR 'Hypercube'> -complete (optional)" << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *configurationFile = argv[i+1];	

// Search for <-o> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-o") == 0) break;
	if (i >= argc - 1) {
      	cout << "\033[0;31mError!\033[0m Not included '-o' parameter." << endl;
        cout << "Executable should be called with: " << argv[0] << " –i <input_file> –c <configuration_file> -o <output_file> -m <method: 'Classic' OR 'LSH' OR 'Hypercube'> -complete (optional)" << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
		return 1;
	}
	char *outputFile = argv[i+1];

// Search for <-m> parameter.
	for (i=1; i < argc - 1; i++)
		if (strcmp(argv[i], "-m") == 0) break;
	if (i < argc - 1) {
        if (i >= argc - 1) {
            cout << "\033[0;31mError!\033[0m Not included '-m' parameter." << endl;
            cout << "Executable should be called with: " << argv[0] << " –i <input_file> –c <configuration_file> -o <output_file> -m <method: 'Classic' OR 'LSH' OR 'Hypercube'> -complete (optional)" << endl;
            cout << "\033[0;31mExit program.\033[0m" << endl;
            return 1;
        }
	}
    if (!strcmp(argv[i+1], (char*) "Classic") && !strcmp(argv[i+1], (char*) "LSH") && !strcmp(argv[i+1], (char*) "Hypercube")) {
        cout << "\033[0;31mError!\033[0m Invalid method.\n" << endl;
        cout << "\033[0;31mExit program.\033[0m" << endl;
        return 1;
    }
	char *method = argv[i+1];

// Search for <-complete> parameter.
    bool complete = false;
	for (i=1; i < argc; i++)
		if (strcmp(argv[i], "-complete") == 0) break;
	if (i < argc) {
        complete = true;
    }

//------------------------------------------------------------------------------------
// Reading input file.

    // Check that input file exists.
    if(access(inputFile, F_OK) == -1) {
        perror("\033[0;31mError\033[0m: Unable to open the input file");
        cout << "\033[0;31mexit program\033[0m" << endl;
        return 1;
    }
    cout << "\033[0;36mRunning Cluster :)\033[0m" << endl << endl;
    // Read input file with PandaC.
    NumC<int>* inputData = PandaC<int>::fromMNIST(inputFile, 50);

//------------------------------------------------------------------------------------
// Reading configuration file.

    // Check that configurationn file exists.
    if(access(configurationFile, F_OK) == -1) {
        perror("\033[0;31mError\033[0m: Unable to open the configurationn file");
        cout << "\033[0;31mexit program\033[0m" << endl;
        return 1;
    }
    // Read configuration file.
    ConfigurationData conf = readConfiguration(configurationFile);
    if (conf.isEmpty()) {
        cout << "\033[0;31mexit program\033[0m" << endl;
        return 1;
    }

//------------------------------------------------------------------------------------
// Making predictions.

    cout << "\033[0;36mComputing clusters...\033[0m" << endl << endl;

//------------------------------------------------------------------------------------
// Call K-Medians and train it.

    Kmedians<int> kMedians(conf.number_of_clusters);
    if (!strcmp(method, (char*) "Classic")) {
        kMedians.fit_transform(inputData, LLOYDS_CLUSTERING);
    } else if (!strcmp(method, (char*) "LSH")) {
        kMedians.fit_transform(inputData, LSH_CLUSTERING);
    } else if (!strcmp(method, (char*) "Hypercube")) {
        kMedians.fit_transform(inputData, HC_CLUSTERING);
    }

//------------------------------------------------------------------------------------
// Execute Predictions and extract results to output file.

    if (extractResults(outputFile, method, complete, &kMedians)) {
        cout << "\033[0;36mResults are extracted in file: \033[0m" << outputFile << endl;
    }

//------------------------------------------------------------------------------------
// End of program.

    //Free allocated Space.
    delete inputData;

    cout << "-----------------------------------------------------------------" << endl;
    cout << "\033[0;36mExit program.\033[0m" << endl;
    return 0;
}

bool extractResults(char* outputFile, char* method, bool complete, Kmedians<int> *kMedians) {

    // Check that output file exists.
    ofstream output(outputFile, fstream::out);
    if (!output.is_open()) {
        perror("\033[0;31mError\033[0m: Unable to open output file");
        return false;
    }

    output << "Algorithm: ";
    if (!strcmp(method, (char*) "Classic")) {
        output << "Lloyds" << endl;
    } else if (!strcmp(method, (char*) "LSH")) {
        output << "Range Search LSH" << endl;
    } else if (!strcmp(method, (char*) "Hypercube")) {
        output << "Range Search Hypercube" << endl;
    }

    NumC<int>* centroids = kMedians->getCentroids();
    vector<Results*> clusters = kMedians->getResults();
    vector<NumCDistType> silhouette = kMedians->getSilhouettes();
    for (int i=0; i < centroids->getRows(); i++) {
        output << "  CLUSTER-" << i+1 << " {size: " << clusters[i]->resultsIndexArray.getCols() << ", centroid: ";
        NumC<int>::print(centroids->getVector(i), output);
        output << "}" << endl; //!+++
    }
    output << "  clustering_time: " << clusters[0]->executionTime << endl; //!+++
    output << "  Silhouette: [ ";
    for (int i=0; i < centroids->getRows(); i++) {
        output << silhouette[i] << ", ";
    }
    output << silhouette[centroids->getRows()] << "]" << endl; //!+++
    if (complete == true) {
        for (int i=0; i < centroids->getRows(); i++) {
            output << "  CLUSTER-" << i+1 << " {centroid: ";
            NumC<int>::print(centroids->getVector(i), output);
            output << ", "; //!+++
            for (int j=0; j < clusters[i]->resultsIndexArray.getCols(); j++) {
                output << clusters[i]->resultsIndexArray.getElement(0, j);
                if (j+1 < clusters[i]->resultsIndexArray.getCols()) output << ", "; //!+++
            }
            output << "}" << endl;
        }
    }

    // Close output file.
    output.close();
    // Free allocated Space.
    // delete centroids;
    for (int i=0; i < (int) clusters.size(); i++) {
        delete clusters[i];
    }

    return true;
}
