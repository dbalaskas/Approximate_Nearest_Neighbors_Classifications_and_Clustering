
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
In our project our objective was to implement the requested classifiers with the minimum complexity. We used priority queues, maps and sparse tables everywhere they were needed. Also we wanted to hold simplicity, user experience and project readability on hight levels.
</h4>

<h3>Project Directories and Files organization</h3>
<h4>
We organized our files in five main directories. In "bin" directory we save the executables and in "build" directory the objective files. "include" contains the .h files and "src" the .cpp files. Finaly, "doc" contains the input/queries/outputs/configuration files that are used during the execution. In main directory you can find the makefile, the README and some scripts which we used to test our executables.
</h4>

<h3>Implementation Details</h3>
<h4>
First of all we used C++ because we wanted to use classes and templates in our project. We wanted to make it abstract so we builded basic classes like PandaC and NumC (copied python) that made easier the coding for us. NumC was used to represent matrix tables and PandaC was used to read the files. In highter level we created classes like 'LSHashing', 'Hypercube' and 'Kmedians' for the classifiers and clustering. 'Kmedians' uses the first two classes during the reverse assignment. 'LSHashing' and 'Hypercube' use the 'HashTable' class that implement their hashtables. When it hashes the vectors, it checks an enum 'HashType' parameter and calls the proper method of an other 'HashFunction' class. To extract the results after range search and KNN the two classifiers use a struct 'Results' that contains the prediction results and time.
</h4>

<h3>How we run the executables</h3>
<h4>
By typing 'make info' you can see how the project compilation is made.
We can execute the executables from master directory by typing './bin/name_of_executables', or by typing './name_of_executables.sh' with the parameters which we used.
</h4>

<h3>How we tested that it works</h3>
<h4>
For our testings we tried toy check all the possible scenarios and we are sure that there is no memory leake
</h4>

<h3>What more our program does</h3>
<h4>
...
</h4>


