MolSim
===

TUM PSE Molecular Dynamics project by Jan Hampe, Alex Hocks, and Johannes Riemenschneider

<h1>Build</h1>

<h3>Required tool installation</h3>

Install xerces-c: <code> sudo apt install libxerces-c-dev</code>  

Install gcc: <code> sudo apt install gcc</code> (at least version 11 is required)  

Install cmake: <code> sudo apt install cmake</code>  

Install make: <code> sudo apt install make</code> 

<h3>Compilation and Build</h3>

Create build directory: <code> mkdir build </code>  

Change directory into build folder: <code> cd build </code>    

Run cmake: <code> cmake .. -B .</code>  

Run make: <code> make</code>

<h2>Build Doxygen Documentation</h2>  

Build with: <code> cmake .. -B . </code>  

Create Doxygen Documentation: <code> make doc_doxygen </code>  

To disable creating Doxygen target run: <code> cmake -DBUILD_DOC=OFF .. -B . </code>  

<h3>Comparing runtime of different algorithms </h3>

The plot comparing the runtime of the different algorithms can be found in the documenation of FLennardJonesCells and FLennardJones since they are responsible for the strategy used. One way to get there would be "Namespaces -> FLennardJonesCells" (sim->physics->force->FLennardJonesCells)

To recreate the results see [benchmarking](#benchmarking).

<h1>Execution</h1>

Execute program with \<input-file\>: <code> ./MolSim \<input file\> </code>  
For the [input file format](#input-files) and [benchmarking](#benchmarking) see the appropriate sections.

<h2>Optional Arguments</h2>

<code> -h </code>: Displays help, same as running <code> ./MolSim </code>  
<code> -dt \<value\> </code>: Sets the time passing between each time step. If omitted a default value is used.  
<code> -et \<value\> </code>: Sets the end time to specified value. If omitted a default value is used.  
<code> -st \<value\> </code>: Sets the start time to the specified value. If omitted a default value is used.  
<code> -sig \<value\> </code>: Sets sigma to the specified value for calculating the [Lennard-Jones-Potential](https://en.wikipedia.org/wiki/Lennard-Jones_potential). If omitted a default value is used.  
<code> -eps \<value\> </code>: Sets epsilon to the specified value for calculating the [Lennard-Jones-Potential](https://en.wikipedia.org/wiki/Lennard-Jones_potential). If omitted a default value is used.  
<code> -o \<name\> </code>: Sets the base name of output files to the specified value. If omitted "result" is used as a default base name. __DO NOT USE A PATH, ONLY A VALID FILE NAME!__  
<code> -of \<path\> </code>: Sets the path to the folder for the output files to be saved. If omitted "./output" is used.  
<code> -llv \<value\> </code>: Sets the log level (0: trace, 1: debug, 2: info 3: warning, 4: error. 5: critical, 6: off). If omitted a default value of 2 is used.
<code> -brown \<value\> </code>: Defines brownian motion globally
<code> -dims \<value\> </code>: Defines the amount of dimensions the simulation should run in. Either 2 or 3.

<h3> Benchmark-specific optionals</h3>
<code> -bench \<value\> </code>: Enable benchmarking. <type> is either 'default' or 'file'. With default an internal hard coded benchmark is performed. With file user supplied input files will be used.
<code> -i \<value\> </code>: Set how many passes should be done for each benchmark.


<h3>Algorithm-defining optionals</h3>
<code> -v \<value\> </code>: Defines the velocity calculation method. Options: 'stoermervelvet', 'stoermervelvetOMP'.
<code> -dims \<value\> </code>: Defines the position calculation method. Options: 'stoermervelvet', 'stoermervelvetOMP'.
<code> -f \<value\> </code>: Defines the force calculation method. Options: 'gravity', 'lennardjones', 'lennardjonesOMP', 'lennardjonescell'.

<code> -lc \<bool\> </code>: Defines whether Linked-Cell algorithm should be used. This algorithm gets used by default. Set <bool\> to 0 for false and to 1 for true
<code> -rc  \<value\> </code>: Sets the cutoff radius for the Linked-Cell algorithm. If omitted a default value is used.
<code> -bMax  \<value\> </code>: Set maximum body size for default benchmark.

<code> -bbox0 \<value\> </code>: Defines the bounding box size in x0 direction.
<code> -bbox1 \<value\> </code>: Defines the bounding box size in x1 direction.
<code> -bbox2 \<value\> </code>: Defines the bounding box size in x2 direction.

<code> -bndTop \<value\> </code>: Defines the boundary condition used at the top border.
<code> -bndBottom \<value\> </code>: Defines the boundary condition used at the bottom border.
<code> -bndFront \<value\> </code>: Defines the boundary condition used at the front border.
<code> -bndRear \<value\> </code>: Defines the boundary condition used at the back border.
<code> -bndRight \<value\> </code>: Defines the boundary condition used at the right border.
<code> -bndLeft \<value\> </code>: Defines the boundary condition used at the left border.

Possible boundary contitions are 'outflow' (removes particles upon crossing domain bounds) and  'reflecting' (reflects particles off of domain bounds, default is outflow).



<h2>Benchmarking</h2>

To benchmark execute program with the following arguments.  
<code> -bench \<type\> \<value\> </code>: Activates benchmark mode. Type can either be "default" or "file". The former uses a hard-coded simulation for easy comparison of execution environments while the latter allows a user-specified input file to be used.  
<code> -i \<value\> </code>: Sets the number of runs to be measured. If omitted a default value is used.  
<code> -bMax \<value\> </code>: Sets the maximum body size. If omitted a default value is used.

Example of testing process:

<code> mkdir build output

cd build

cmake ..

make

./MolSim ../input/square1.txt -dt 0.0005 -et 0.5 -lc 1 -bbox0 50.0 -bbox1 50.0 -f lennardjonescell -rc 3.0 -bench file -i 10 > ../output/lc\_square1.txt
 </code>

To test All-Pairs algorithm instead execute:
<code>
./MolSim ../input/square1.txt -dt 0.0005 -et 0.5 -lc 0 -bbox0 50.0 -bbox1 50.0 -f lennardjones -rc 3.0 -bench file -i 10 > ../output/ap\_square1.txt
</code>

Bigger tests can be run by changing bbox, the input file and the output file accordingly


<h2>Input Files</h2>

The input files of the current program should be given as an .xml-file conforming to <code>../input/XMLFormat.xsd </code>.

See <code>../input/test.xml</code> to get an example of a valid input files.

For benchmarking purposes (when -bench is set as a command line argument) input files can also be given in the following format:

Lines of comment begin with a '#' and are only allowed at the beginning of the file.  
The first non-commented line must be an integer indicating the number of bodies to be specified. Each body takes up one line.  
To create a single particle, specify its xyz-coordinates, velocity, and mass like so: <code>x.x y.y z.z x.x y.y z.z m.m</code>.  
To make creation of complex bodies more convienient, one can create one by specifying its coodinates of its front-left corner, velocity, mass, shape, dimensions, and the distance between each individual particle like so: <code>x.x y.y z.z x.x y.y z.z m.m shape x y z d.d</code>.  Currently supported shapes are Cuboids and Particles (There is no functional difference between declaring a single particle the first or second way). Capitalization doesn't matter.   
If the simulation uses the [Lennard-Jones Potential](https://en.wikipedia.org/wiki/Lennard-Jones_potential), sigma and epsilon can optionally be specified in the input file. Also the average velocity of the Brownian Motion as well as the number of dimensions (2 or 3) can be set here.  
An example input file can be found under <code>./input/example-file.txt</code>.

If the program is not executed in bench-mode these input files won't be read in properly. This behaviour will change with assignment 4.

<h2>Tests</h2>

If this is the first time building the tests, then the doctest repo has to be cloned.
This will be done automatically by make.
To do this, follow the regular build instructions.  

After this the tests can be built (also in the build directory) by running: <code> make runtest </code>

To enable the DEBUG flag, use: <code> make CXX_FLAGS+="-DDEBUG -std=c++20" runtest </code>

Run tests with <code>ctest</code>

<h1> Presentation </h1>
Each weeks presentations (and the corresponding .tex-files) can be found in the Jo/presentation branch of this repository.
