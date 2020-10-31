
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

<h3>What more our program does</h3>
<h4>
...
</h4>


