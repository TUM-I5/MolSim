MolSim PSEMolDyn_GroupG
=======================

Code for the practical course *PSE: Molecular Dynamics* by group G.

Members: <br />
Alexandra Marquardt <br />
Matteo Wohlrapp <br />
Michael Borisov <br />
Group Id: G

Link to the repository: https://github.com/SecTedd/PSEMolDyn_GroupG <br />
Branch: assignment2 <br />
Commit Id: # <br />


Video: <br />
The two cuboids colide. In the beginning you can see the particles moving aroung a little bit, this is caused by the brownian motion and works as expected. <br />
Watch video at 60 times speed or so. 

## Build and run: 

### Build without Doxygen
1. Navigate into the build folder: `cd build` 
2. Run `cmake ..` to create the makefile. We use gcc 11.2.0 as compiler.
3. Run `make` to generate the executable and tests.

### Build with Doxygen
1. Navigate into the build folder: `cd build`
2. Run `cmake -D BUILD_DOC=ON ..` to create the makefile. We use gcc 11.2.0 as the compiler.
3. Run `make` to generate the executable and tests and `make doc_doxygen` to generate the documentation. 

### Run 
After building the project you can run the executable 
1. Navigate into the build folder: `cd build` 
2. The application can be run from the console (but only with one input file). Call ./MolSim -h to print the help text: <br />
-f <filename> .......... The path to an input file. If not specified and no cuboids are generated, no particles appear in the simulation. <br />
-t <end_time> .......... The end time of the simulation. If not specified, 100 is used. <br />
-d <delta_t> ........... The size of the time steps in the simulation. If not specified 0.014 is used. <br />
-l <log_level>.......... Activate '1' or deactivate '0' the loggers. If not specified, the loggers are activated. <br />
-m ..................... Enter the console menu, here you can read in files, create cuboids and re-run the program with the same parameters. <br />
-h ..................... Help <br />
The -f option automatically distinguishes between old input files and new cuboid files <br />
3. The application also allows you to enter a interactive menu where you can read in multiple files, start the simulation multiple times, etc. Run ./MolSim -m to enter the menu, a help message is shown: <br />
MolSim Group G > Welcome to the menu of the MolSim application. In here, you can set parameters of the application, read in multiple inputs, and finally run it <br />
MolSim Group G > -f <filename> .......... The path to an input file. If not specified no particles are generated <br />
MolSim Group G > -t <end_time> .......... The end time of the simulation. If not specified, 100 is used <br />
MolSim Group G > -d <delta_t> ........... The size of the time steps in the simulation. If not specified 0.014 is used <br />
MolSim Group G > -x ..................... Deletes all particles from the simulation <br />
MolSim Group G > -r ..................... Run the program with the currently set values <br />
MolSim Group G > -h ..................... Help for the menu <br />
MolSim Group G > -q ..................... Quit the program <br />
The -f option automatically distinguishes between old input files and new cuboid files <br />
4. The generated files can be found at ~/build/outputXXX where **XXX** is the output type, e.g. VTK or XYZ
5. **Warning** the contents of the output folders will be overwritten in every run of the simulation!

### Run tests
1. Navigate into the build folder: `cd build`
2. Run `ctest` to execute all unit tests.
3. The result of the unit tests are printed to the console.

## Logging: 
For logging we use spdlog. The logs are written to files which can be found in the **/logs/** folder. The logs are separated into logic and memory logs. Logic logs are used to log events in the program flow. Within the logic logs, there is the distinction between input, output and simulation. Memory logs on the other hand document the construction and destruction of objects and therefore help to detect and prevent memory leaks.

## Structure: 
```
./
├── build
├── cmake
│   └── modules
│       ├── doxygen.cmake
│       ├── googletest.cmake
│       └── spdlog.cmake
├── CMakeLists.txt
├── Doxyfile
├── input
│   ├── eingabe-cuboid1.txt
│   ├── eingabe-cuboid2.txt
│   └── eingabe-sonne.txt
├── libs
├── logs
│   ├── input.txt
│   ├── memory.text
│   ├── output.txt
│   └── simulation.txt
├── README.md
├── src
│   ├── ConsoleMenu.cpp
│   ├── ConsoleMenu.h
│   ├── inputReader
│   │   ├── CuboidInputReader.cpp
│   │   ├── CuboidInputReader.h
│   │   ├── FileReader.cpp
│   │   ├── FileReader.h
│   │   ├── InputFacade.cpp
│   │   ├── InputFacade.h
│   │   ├── InputReader.cpp
│   │   └── InputReader.h
│   ├── model
│   │   ├── Cuboid.cpp
│   │   ├── Cuboid.h
│   │   ├── ParticleContainer.cpp
│   │   ├── ParticleContainer.h
│   │   ├── Particle.cpp
│   │   ├── Particle.h
│   │   ├── ProgramParameters.cpp
│   │   └── ProgramParameters.h
│   ├── MolSim.cpp
│   ├── outputWriter
│   │   ├── OutputFacade.cpp
│   │   ├── OutputFacade.h
│   │   ├── vtk-unstructured.cpp
│   │   ├── vtk-unstructured.h
│   │   ├── vtk-unstructured.xsd
│   │   ├── VTKWriter.cpp
│   │   ├── VTKWriter.h
│   │   ├── XYZWriter.cpp
│   │   └── XYZWriter.h
│   ├── simulation
│   │   ├── ForceCalculation.cpp
│   │   ├── ForceCalculation.h
│   │   ├── GravitationalForce.cpp
│   │   ├── GravitationalForce.h
│   │   ├── LennardJonesForce.cpp
│   │   ├── LennardJonesForce.h
│   │   ├── Simulation.cpp
│   │   └── Simulation.h
│   └── utils
│       ├── ArrayUtils.h
│       ├── Input.h
│       └── MaxwellBoltzmannDistribution.h
└── tests
    ├── CuboidInputReader_test.cc
    ├── eingabe-cuboid.txt
    ├── LennardJonesForce_test.cc
    ├── main.cc
    └── ParticleContainer_test.cc
```