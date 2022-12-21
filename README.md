MolSim
===

TUM PSE Molecular Dynamics project by Jan Hampe, Alex Hocks, and Johannes Riemenschneider

## Build

### Required tool installation

* Install xerces-c: ` sudo apt install libxerces-c-dev`
* Install gcc: ` sudo apt install gcc` (at least version 11 is required)
* Install cmake: ` sudo apt install cmake`
* Install make: ` sudo apt install make`

### Compilation and Build

* Create build directory: ` mkdir build `
* Change directory into build folder: ` cd build `
* Run cmake: ` cmake .. -B .`
* Run make: ` make`

## Build Doxygen Documentation  

* Build with: ` cmake .. -B . `
* Create Doxygen Documentation: ` make doc_doxygen `
* To disable creating Doxygen target run: ` cmake -DBUILD_DOC=OFF .. -B . `

### Comparing runtime of different algorithms

The plot comparing the runtime of the different algorithms can be found in the documenation of FLennardJonesCells and FLennardJones since they are responsible for the strategy used. One way to get there would be "Namespaces -> FLennardJonesCells" (sim->physics->force->FLennardJonesCells)<br>

To recreate the results see [benchmarking](#benchmarking).

### Profiling
* Execute ` BenchMolSim ` instead of ` MolSim ` with the desired input file and parameters (see [Execution](#execution))
* Run `gprof BenchMolSim gmon.out > profile-data.txt `
* The profile data can now be found at profile-data.txt

## Execution

Execute program with <input-file>: ` ./MolSim <input-file> `  
For the [input file format](#input-files) and [benchmarking](#benchmarking) see the appropriate sections.

__WARNING:__ When using linked cell algorithm linked cell mode has to be activated by setting `-lc 1`

### Execution Examples
1. running with all default values: `./MolSim <input-file>`
2. setting delta_t and end time: `./MolSim <input-file> -dt 0.0002 -et 1.0`
3. setting log level: `./MolSim <input-file> -llv 0`
4. using linked cell: `./MolSim <input-file> -dt 0.00005 -et 10.0 -lc 1 -f lennardjonescell`

## Optional Arguments

* `-h`: Displays help, same as running ` ./MolSim `  
* ` -dt <value> `: Sets the time passing between each time step. If omitted a default value is used.  
* ` -et <value> `: Sets the end time to specified value. If omitted a default value is used.  
* ` -st <value> `: Sets the start time to the specified value. If omitted a default value is used.  
* ` -sig <value> `: Sets sigma to the specified value for calculating the [Lennard-Jones-Potential](https://en.wikipedia.org/wiki/Lennard-Jones_potential). If omitted a default value is used.  
* ` -eps <value> `: Sets epsilon to the specified value for calculating the [Lennard-Jones-Potential](https://en.wikipedia.org/wiki/Lennard-Jones_potential). If omitted a default value is used.  
* ` -o <name> `: Sets the base name of output files to the specified value. If omitted "result" is used as a default base name. __DO NOT USE A PATH, ONLY A VALID FILE NAME!__  
* ` -of <path> `: Sets the path to the folder for the output files to be saved. If omitted "./output" is used.  
* ` -llv <value> `: Sets the log level (0: trace, 1: debug, 2: info 3: warning, 4: error. 5: critical, 6: off). If omitted a default value of 2 is used.
* ` -brown <value> `: Defines brownian motion globally
* ` -dims <value> `: Defines the amount of dimensions the simulation should run in. Either 2 or 3.

### Benchmark-specific optionals
* ` -bench <value> `: Enable benchmarking. `<type>` is either 'default' or 'file'. With default an internal hard coded benchmark is performed. With file user supplied input files will be used.
* ` -i <value> `: Set how many passes should be done for each benchmark.


### Algorithm-defining optionals
* ` -v <value> `: Defines the velocity calculation method. Options: 'stoermervelvet', 'stoermervelvetOMP'.
* ` -dims <value> `: Defines the position calculation method. Options: 'stoermervelvet', 'stoermervelvetOMP'.
* ` -f <value> `: Defines the force calculation method. Options: 'gravity', 'lennardjones', 'lennardjonesOMP', 'lennardjonescell'.

* ` -lc <bool> `: Defines whether Linked-Cell algorithm should be used. This algorithm gets used by default. Set `<bool>` to 0 for false and to 1 for true
* ` -rc  <value> `: Sets the cutoff radius for the Linked-Cell algorithm. If omitted a default value is used.
* ` -bMax  <value> `: Set maximum body size for default benchmark.

* ` -bbox0 <value> `: Defines the bounding box size in x0 direction.
* ` -bbox1 <value> `: Defines the bounding box size in x1 direction.
* ` -bbox2 <value> `: Defines the bounding box size in x2 direction.

* ` -bndTop <value> `: Defines the boundary condition used at the top border.
* ` -bndBottom <value> `: Defines the boundary condition used at the bottom border.
* ` -bndFront <value> `: Defines the boundary condition used at the front border.
* ` -bndRear <value> `: Defines the boundary condition used at the back border.
* ` -bndRight <value> `: Defines the boundary condition used at the right border.
* ` -bndLeft <value> `: Defines the boundary condition used at the left border.

Possible boundary contitions are 'outflow' (removes particles upon crossing domain bounds) and  'reflecting' (reflects particles off of domain bounds, default is outflow).

## Benchmarking

To benchmark execute program with the following arguments.  
* ` -bench <type> `: Activates benchmark mode. Type can either be "default" or "file". The former uses a hard-coded simulation for easy comparison of execution environments while the latter allows a user-specified input file to be used.  
* ` -i <value> `: Sets the number of runs to be measured. If omitted a default value is used.  
* ` -bMax <value> `: Sets the maximum body size. If omitted a default value is used.

Example of testing process:

1. ` mkdir build output `
2. ` cd build `
3. ` cmake .. `
4. ` make `
5. ` ./MolSim ../input/square1.txt -dt 0.0005 -et 0.5 -lc 1 -bbox0 50.0 -bbox1 50.0 -f lennardjonescell -rc 3.0 -bench file -i 10 > ../output/lc_square1.txt `

To test All-Pairs algorithm instead execute:
* ` ./MolSim ../input/square1.txt -dt 0.0005 -et 0.5 -lc 0 -bbox0 50.0 -bbox1 50.0 -f lennardjones -rc 3.0 -bench file -i 10 > ../output/ap_square1.txt `
Bigger tests can be run by changing bbox, the input file and the output file accordingly


## Input Files

The input files of the current program should be given as an .xml-file conforming to [XMLFormat.xsd](https://github.com/penguincarol/Alex_Jan_Johannes_Mol_Sim/blob/master/input/XMLFormat.xsd).
See [test.xml](https://github.com/penguincarol/Alex_Jan_Johannes_Mol_Sim/blob/master/input/test.xml) to get an example of a valid input files.

For benchmarking purposes (when -bench is set as a command line argument) input files can also be given in the following format:
* Lines of comment begin with a '#' and are only allowed at the beginning of the file.  
* The first non-commented line must be an integer indicating the number of bodies to be specified. Each body takes up one line.  
* To create a single particle, specify its xyz-coordinates, velocity, and mass like so: `x.x y.y z.z x.x y.y z.z m.m`.  
* To make creation of complex bodies more convienient, one can create one by specifying its coodinates of its front-left corner, velocity, mass, shape, dimensions, and the distance between each individual particle like so: `x.x y.y z.z x.x y.y z.z m.m shape x y z d.d`.  Currently supported shapes are Cuboids and Particles (There is no functional difference between declaring a single particle the first or second way). Capitalization doesn't matter.   
* If the simulation uses the [Lennard-Jones Potential](https://en.wikipedia.org/wiki/Lennard-Jones_potential), sigma and epsilon can optionally be specified in the input file. Also the average velocity of the Brownian Motion as well as the number of dimensions (2 or 3) can be set here.  
* An example input file can be found under [example-file.txt](https://github.com/penguincarol/Alex_Jan_Johannes_Mol_Sim/blob/master/input/example-file.txt).

If the program is not executed in bench-mode these input files won't be read in properly. This behaviour will change with assignment 4.

## Tests

If this is the first time building the tests, then the doctest repo has to be cloned.
This will be done automatically by make.

To do this, follow the regular build instructions:  

1. After this the tests can be built (also in the build directory) by running: ` make runtest `
2. To enable the DEBUG flag, use: ` make CXX_FLAGS+="-DDEBUG -std=c++20" runtest `
3. Run tests with `ctest`

# Presentation 

Each week's presentations (and the corresponding .tex-files) can be found in the Jo/presentation branch of this repository.
