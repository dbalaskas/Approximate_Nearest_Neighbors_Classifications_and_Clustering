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

typedef struct ConfigurationData{
    int number_of_clusters; // K of K-medians
    int L;                  // default: L=3
    int k;                  // k of LSH for vectors, default: 4
    int M;                  // M of Hypercube, default: 10
    int d;                  // k of Hypercube, default: 3
    int probes;             // probes of Hypercube, default: 2
  
    ConfigurationData()
     :number_of_clusters{-1}, 
     L{3}, 
     k{4}, 
     M{10},
     d{3}, 
     probes{2} {};

    ~ConfigurationData() {
        number_of_clusters = -1;
    };

    bool isEmpty() { return number_of_clusters == -1; };
    void print() {
        cout << "-------------------------" << endl;
        cout << "Configuration: " << endl;
        cout << "  number_of_clusters: " << number_of_clusters << endl;
        cout << "  L: " << L << endl;
        cout << "  k: " << k << endl;
        cout << "  M: " << M << endl;
        cout << "  d: " << d << endl;
        cout << "  probes: " << probes << endl;
        cout << "-------------------------" << endl;
    }

} ConfigurationData;

// Extracts the results to output file.
void extractResults(char* outputFile, char* method, bool complete, Results* results, Results *true_results);
ConfigurationData readConfiguration(char* configurationFile);

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
	if (i < argc - 1) {
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
    NumC<int>* inputData = PandaC<int>::fromMNIST(inputFile);

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

Results *knn_results, *true_results;
// vector<Results*> r_results;

cout << "\033[0;36mComputing clusters...\033[0m" << endl << endl;
//------------------------------------------------------------------------------------
// Call LSHashing classifier and train it.

LSHashing<int> lsh(conf.L, conf.k, 50000);
lsh.fit_transform(inputData);

// //------------------------------------------------------------------------------------
// // Call exhaustive knn classifier and train it.

// ExhaustiveKnn<int> exhaustive_knn(inputData, N);

// //------------------------------------------------------------------------------------
// // Execute Predictions and extract results to output file.

// // Execute k-NN prediction.
// knn_results = lsh.predict_knn(queryData, N);
// // Execute Exhaustive KNN search.
// true_results = exhaustive_knn.predict_knn(queryData);
// // Execute Range Search.
// r_results = lsh.predict_rs(queryData, R);
// // Extract results on output file.
extractResults(outputFile, method, complete, knn_results, true_results);
cout << "Results are extracted in file: " << outputFile << endl;

//------------------------------------------------------------------------------------
// End of program.

    //Free allocated Space.
    delete inputData;
    delete knn_results;
    delete true_results;
    // for (int i=0; i < (int) r_results.size(); i++) {
    //     delete r_results[i];
    // }

    cout << "-----------------------------------------------------------------" << endl;
    cout << "\033[0;36mExit program.\033[0m" << endl;
    return 0;
}

void extractResults(char* outputFile, char* method, bool complete, Results* results, Results *true_results) {
    NumC<int>* inputDatalabels = PandaC<int>::fromMNISTlabels((char*) "./doc/input/train-labels-idx1-ubyte");

    ofstream output(outputFile, ios::out);
    // Check that output file exists.
    if (!output.is_open()) {
        perror("\033[0;31mError\033[0m: Unable to open output file");
        return;
    }

    
    output << "Algorithm: ";
    if (!strcmp(method, (char*) "Classic")) {
        cout << "Lloyds" << endl;
    } else if (!strcmp(method, (char*) "LSH")) {
        cout << "Range Search LSH" << endl;
    } else if (!strcmp(method, (char*) "Hypercube")) {
        cout << "Range Search Hypercube" << endl;
    }
    // for (int i=0; i < numOfClusters; i++) {
    //     output << "  CLUSTER-" << j+1 << " {size: " << /*/*/ << ", centroid: " << /*/*/ << "}" << endl; //!+++
    // }
    // output << "  clustering_time: " << /*/*/ << endl; //!+++
    // output << "  Silhouette: [" << /*/*/ << "]" << endl; //!+++
    // if (complete == true) {
    //     for (int i=0; i < numOfClusters; i++) {
    //         output << "  CLUSTER-" << j+1 << " {centroid: " << /*/*/ << ", "; //!+++
    //         for (int j=0; j < cluster_contents; j++) {
    //             cout << /*...*/ << ", "; //!+++
    //         }
    //         output << "}" << endl;
    //     }
    // }

    // Close output file.
    output.close();
    delete inputDatalabels;
}

ConfigurationData readConfiguration(char* configurationFile) {
    ConfigurationData confData;
    FILE *conf = fopen(configurationFile, "r");
    
    char line[128];
    char *command;
    int value;
    while(fgets(line,sizeof(line),conf) != NULL) {
        if (line[0] == '#') {
            // Comment
            continue;
        }
        if (strlen(line) > 1) {
            // cout << line;
    		command = strtok(line," : ");
    		value = atoi(strtok(NULL,"\n"));
        }
        // cout << "<" << command << ">: " << value << endl;
        if (!strcmp(command, (char*) "number_of_clusters")) {
            confData.number_of_clusters = value;
        } else if (!strcmp(command, (char*) "number_of_vector_hash_tables")) {
            confData.L = value;
        } else if (!strcmp(command, (char*) "number_of_vector_hash_functions")) {
            confData.k = value;
        } else if (!strcmp(command, (char*) "max_number_M_hypercube")) {
            confData.M = value;
        } else if (!strcmp(command, (char*) "number_of_hypercube_dimensions")) {
            confData.d = value;
        } else if (!strcmp(command, (char*) "number_of_probes")) {
            confData.probes = value;
        } else {
            // Not accepted configuration
        }
    }
    if (!feof(conf)) {
      	cout << "\033[0;31mError!\033[0m Bad configuration file." << endl;
        fclose(conf);
        return ConfigurationData();
    }

    fclose(conf);
    confData.print();
    return confData;
}
