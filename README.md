
<p style="text-align: center;">
    <img src="./doc/images/di_uoa.png" alt="UOA">
    <h1>University of Athens</h1>
    <h2>Department of Informatics and Telecomunications</h2>
</p>

<h3>Dionysis Taxiarchis Balaskas - 1115201700094</h3>
<h3>Andreas Giannoutsos - 1115201700021</h3>
<br><br>


<h3>Introduction to our project (info, goals, complexity, speed, results, simplicity, abstractiveness)</h3>
<h4>
In this project we implement two classifiers LSH and Hypercube, with C++, compare their 
nearest neighbors' prediction with exhaustive KNN and print their range search results. 
On second phase we implement a K-medians clusterer which initialize the centroids with 
kmeans++, finds the centroids with medians function and gets the clusters based on either 
classic Lloyd's function either LSH/Hypercube reverse assignment.
Our objective was to implement the classifiers based on simplicity and abstractiveness,
with the minimum possible complexity. We used priority queues, hashtables and sparse tables, in order to reduce the time that it takes for the predictions to be made.
It's important that we wanted to create a more approachable and easy user experience. So we added as many validity checks, to reduce the faults from user's mistakes.
</h4>

<h3>How we run the executables</h3>
<h4>
In main directory anyone can find makefile. By typing 'make info' we can see the available
commands. 'make' compiles all the executables and we can run them by typing 
'./bin/name_of_executable' or './name_of_executables.sh'.
</h4>

<h3>How we tested that it works</h3>
<h4>
We tried to test all possible scenarios, with many different parameters. We checked for 
memory leak with valgrind.
</h4>

<h3>Project Directories and Files organization</h3>
<h4>
We tried to organize our project files by type.
Our project is organized with:
    
1. bin: the directory where the executables are saved.

2. build: the directory where the objective files are.

3. include: contains the '.h' files.

4. src: contains the '.cpp' files.

5. doc: contains data files like input/query/output/configuration.

In main directory there are makefile, README.md and some '.sh' scripts that we used to 
test our executables.
</h4>

<h3>Implementation Details</h3>
<h4>
One of the many reasons that we used C++ was that we wanted to make use of templates and 
classes, in order to make the project abstract. We created some basic classes like PandaC 
and NumC that we used multiple times in the project. The first represents matrixes like 
NumPy in python and the second is used to read MNIST files. In highter level 
we created LSHashing, Hypercube and Kmedians classes for the classifiers and the 
clusterer. The last one uses the first two on reverse assignment. LSHashing and Hypercube 
use HashTable class that hashes each vector with HashFuncion class based on enum 
'HashType' parameter.
</h4>

<h3>Modules Details</h3>
<h5>
    The program is implemented using modules that perform a separate task. Some are independent, while some need the help of other modules to complete their work.
    The following is a description of all the modules and their basic functions
</h5>

<h4>
    
1. PandaC:
    In this module the MNIST files of the dataset are read. The bytes are reversed for the little endian systems. You can also read the labels file 

2. NumC

    In this module an attempt has been made to copy Python's NumPy and it is the module with which we manage tables and data.
This module is used by all others to store data.
It has a template and can store square tables of different types of data int, double, long. The data is stored in a one-dimensional array and then for the two-dimensional simulation the indicators are multiplied accordingly.
From this module the arrays of the array can be used autonomously in vector format which is indicated in this array of the array.

    Additionally a function of the module is the sparse storage and representation of data. As the data has a lot of zeros and is fine with each other, a sparse storage significantly reduces memory usage as well as the number of calculations.
    The distance between vectors is done with sparse subtraction, in which only the elements of the array are removed and the zeros are ignored. Experimentally we have observed an improvement of 30% with this method.

    In addition to this module, arithmetic operations are performed on the data such as the creation of a random table, the squaring of the elements, the normalization, the addition of elements and more.


3. ExhaustiveKnn

    In this module the exhaustive search of the nearest neighbor is performed.
    The data is stored in a NumC table and then for each search at each point the distance is calculated by sparse method. The results are stored in priority queue and returned to the nearest K

4. HashFunction

    In this module the calculations of the hash function for lsh and for hc are done. The random values of Si and Fi intervals for lsh and hc are stored in NumC, respectively.

5. HashTable

    The hashFunction implements the hash tables in duplicate std :: vector and for each bucket there is the struct Node which contains the information about the vector but also the values of the hash function, which is needed for the control in lsh

6. ResultsComporator 

    This module is used by all the following classifiers. Here is the registration of results in priority queue and the promotion of the k closest-best. The results are returned to struct Results which has 2 NumC tables that store the indexes and distances of the vectors as well as a table for the time it took for each query to run.

7. HyperCube 

    This is where the Hypercube algorithm is implemented. This module uses all of the above except PandaC. With all the above functions it performs a search in each bucket of the supercube and stores the results in the struct Results.
    In addition, a retrospective algorithm is implemented to find the buckets that have the required humming distance.

8. LSHashing 

    This is where the Hypercube algorithm is implemented. This module uses all of the above except PandaC. With all the above functions it performs a search in each bucket of the supercube and stores the results in the struct Results.
In addition, a retrospective algorithm is implemented to find the buckets that have the required humming distance.

9. Kmedians 

    In this module all the clustering algorithms are implemented. The module uses NumC but also HuperCube and LSHashing for the reverse assignment.
    When the choice is given for Lloyds transform, exhaustively for each centroid its average is calculated and its price is renewed. The algorithm stops in 20 repetitions or if the error difference is less than 1e-5%.
    
    For the reverse assignment, HuperCube and LSHashing implement the algorithm as they have information about the buckets and return to Kmedians the results with which the centroids are renewed.
    
    The centroids are initialized with the Kmeans ++ algorithm and the Cluster is scored with the Metric Silhouette which exhaustively calculates all distances with all nearby points.

</h4>
  




<h3>What more our program does</h3>
<h4>
...
</h4>


