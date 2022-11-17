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

Build with: <code> cmake .. -B . </code>  

Create Doxygen Documentation: <code> make doc_doxygen </code>  

To disable creating Doxygen target run: <code> cmake -DBUILD_DOC=OFF .. -B . </code>  

<h1> Execution </h1>

Execute program with \<input-file\>: <code> ./MolSim \<input file\> </code>  
For accepted input file formats and benchmarking see the appropriate sections.

<h2> Optional Arguments </h2>

<code> -h </code>: Displays help, same as running <code> ./MolSim </code>
<code> -dt \<value\> </code>: Sets the time passing between each time step. If omitted a default value is used.  
<code> -et \<value\> </code>: Sets the end time to specified value. If omitted a default value is used.  
<code> -st \<value\> </code>: Sets the start time to the specified value. If omitted a default value is used.  
<code> -sig \<value\> </code>: Sets the sigma to the specified value for. If omitted a default value is used.



<h2> Tests </h2>
If this is the first time building the tests, then the doctest repo has to be cloned.
This will be done automatically by make.
To do this, follow the regular build instructions.  

After this the tests can be built (also in the build directory) by calling: <code> make runtest </code>

To enable the DEBUG flag, use: <code> make CXX_FLAGS+="-DDEBUG -std=c++20" runtest </code>

<h1> Presentation </h1>
The presentation (and the corresponding .tex-files) can be found at presentation/200805_TUM_LaTex-Vorlagenpaket/Week1_Presentation.pdf
