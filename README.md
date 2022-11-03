MolSim
===

The Molecular Dynamics teaching code.

<h1> Build </h1>

Install xerces-c: <code> sudo apt install libxerces-c-dev</code>  

Install gcc: <code> sudo apt install gcc</code> (at least version 11 is required)  

Install cmake: <code> sudo apt install cmake</code>  

Install make: <code> sudo apt install make</code>  

Change directory into build folder: <code> cd build </code>  

Run cmake: <code> cmake .. -B .</code>  

Run make: <code> make</code>

Create Doxygen Documentation: make doc_doxygen

Disable creating Doxygen target: cmake DBUILD_DOC=OFF . -B ..

<h1> Execution </h1>

Run simulation with \<input-file\>: <code> ./MolSim \<input-file\> [-et \<end-time\>] [-dt \<timeslice length\>]</code>

<h2> Testing </h2>
If this is the first time of building the tests then the doctest repo has to be cloned.
This will be done automatically by make.
To do this, run in the build directory: <br> 1. <code> cmake ..</code> <br> 2. <code> make </code>

After this the tests can be built (also in the build directory) by calling: <code> make test </code>

To enable DEBUG flag, use: <code> make CXX_FLAGS+="-DDEBUG -std=c++20" test </code>

<h3> Presentation </h3>
The presentation (and the corresponding .tex-files) can be found at presentation/200805_TUM_LaTex-Vorlagenpaket/Week1_Presentation.pdf