MolSim
===

TUM PSE Molecular Dynamics project by Jan Hampe, Alex Hocks, and Johannes Riemenschneider

<h1> Build </h1>

<h3> Required tool installation </h3>

Install xerces-c: <code> sudo apt install libxerces-c-dev</code>  

Install gcc: <code> sudo apt install gcc</code> (at least version 11 is required)  

Install cmake: <code> sudo apt install cmake</code>  

Install make: <code> sudo apt install make</code> 

<h3> Compilation and Build </h3>

Create build directory: <code> mkdir build </code>  

Change directory into build folder: <code> cd build </code>    

Run cmake: <code> cmake .. -B .</code>  

Run make: <code> make</code>

<h2> Build Doxygen Documentation </h2>  
  
Disable creating Doxygen target: <code> cmake -DBUILD_DOC=OFF .. -B . </code>  

Create Doxygen Documentation: <code> make doc_doxygen </code>  

<h1> Execution </h1>

Run simulation with \<input-file\>: <code> ./MolSim \<input-file\> [-et \<end-time\>] [-dt \<timeslice length\>]</code>

<h2> Testing </h2>
If this is the first time building the tests, then the doctest repo has to be cloned.
This will be done automatically by make.
To do this, in the build directory, run: <br> 1. <code> cmake ..</code> <br> 2. <code> make </code>

After this the tests can be built (also in the build directory) by calling: <code> make test </code>

To enable the DEBUG flag, use: <code> make CXX_FLAGS+="-DDEBUG -std=c++20" test </code>

<h3> Presentation </h3>
The presentation (and the corresponding .tex-files) can be found at presentation/200805_TUM_LaTex-Vorlagenpaket/Week1_Presentation.pdf
