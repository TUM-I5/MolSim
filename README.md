# MolSim PSEMolDyn_GroupG

Code for the practical course *PSE: Molecular Dynamics* by group G.

Members: <br />
Alexandra Marquardt <br />
Matteo Wohlrapp <br />
Michael Borisov <br />
Group Id: G

Link to the repository: https://github.com/SecTedd/PSEMolDyn_GroupG <br />
Branch: assignment3 <br />
Commit Id: # <br />

Video: <br />

1. Video <br />
   CuboidCollide.avi is almost the same video as for last week, but with more particles and the use of the new Linked Cell Algorithm. <br />
   The Linked Cell Algorithm makes it possible to have many more particles and still a reasonable runtime. <br />
   For Details about performance and the implementation please review the slides. <br />
   <br />
2. Video <br />
   FallingDrop.avi shows our falling drop simulation. Here you can see how a sphere, representing our water drop, falls onto an invisible boundary and splashes around. <br />
   Gravity is not implemented yet, thats why the water drop goes back up in the end of the video. <br />

## Build and run

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

1. Familiarize yourself with the provided .xml file located in the /input/ folder and the .xsd file located in the /xsd/ folder. <br />
   XML-Files are used to specify different parameters. <br />
2. Create a new XML-File or use the provided one and specify all the input parameters that are needed. <br />
3. Navigate into the build folder: `cd build`
4. The application can be run from the console. Run `./MolSim -h` to print the help text. <br />
   The -f option automatically distinguishes between different input files, such as xml, cuboid, sphere etc. <br />
5. The application also allows you to enter an interactive menu where you can read in multiple files, start the simulation multiple times, etc. Run `./MolSim -m` to enter the menu, a help message is shown. <br />
   The -f option automatically distinguishes between different input files, such as xml, cuboid, sphere etc. <br />
6. The generated VTK-Files can be found at the path you specified in the xml. <br />
7. **Warning** the contents of the output folders will be overwritten if you run the simulation multiple times with the same output folder path! <br />

### Run tests

1. Navigate into the build folder: `cd build`
2. Run `ctest` to execute all unit tests.
3. The result of the unit tests are printed to the console.

## Logging

For logging we use spdlog. <br />
The logs are written to files which can be found in the **/logs/** folder or to the console (see `./MolSim -h`). The logs are separated into logic and memory logs. <br />
Logic logs are used to log events in the program flow. Within the logic logs, there is the distinction between input, output and simulation. <br />
Memory logs on the other hand document the construction and destruction of objects and therefore help to detect and prevent memory leaks. <br />

## Plots
![SecPerIt](./plots/Sec-per-it.png "Seconds per iteration")
![MUPS](./plots/MUPS.png "MUPS")

## Structure: 
```
./
├── CMakeLists.txt
├── Doxyfile
├── README.md
├── build
├── cmake
│   └── modules
│       ├── doxygen.cmake
│       ├── googletest.cmake
│       └── spdlog.cmake
├── input
│   ├── Simulation.xml
│   ├── eingabe-cuboid1.txt
│   ├── eingabe-cuboid2.txt
│   ├── eingabe-cuboid3.txt
│   ├── eingabe-cuboid4.txt
│   ├── eingabe-sonne.txt
│   ├── eingabe-sphere.txt
│   ├── falling-drop.txt
│   ├── rectangle-1000.txt
│   ├── rectangle-2000.txt
│   ├── rectangle-4000.txt
│   └── rectangle-8000.txt
├── libs
├── plots
├── src
│   ├── ConsoleMenu.cpp
│   ├── ConsoleMenu.h
│   ├── MolSim.cpp
│   ├── inputReader
│   │   ├── CuboidInputReader.cpp
│   │   ├── CuboidInputReader.h
│   │   ├── FileReader.cpp
│   │   ├── FileReader.h
│   │   ├── InputFacade.cpp
│   │   ├── InputFacade.h
│   │   ├── InputReader.cpp
│   │   ├── InputReader.h
│   │   ├── SphereInputReader.cpp
│   │   ├── SphereInputReader.h
│   │   ├── XMLInputReader.cpp
│   │   └── XMLInputReader.h
│   ├── model
│   │   ├── Cuboid.cpp
│   │   ├── Cuboid.h
│   │   ├── DirectSumParticleContainer.cpp
│   │   ├── DirectSumParticleContainer.h
│   │   ├── LinkedCellParticleContainer.cpp
│   │   ├── LinkedCellParticleContainer.h
│   │   ├── Particle.cpp
│   │   ├── Particle.h
│   │   ├── ParticleCell.cpp
│   │   ├── ParticleCell.h
│   │   ├── ParticleContainer.h
│   │   ├── ProgramParameters.cpp
│   │   ├── ProgramParameters.h
│   │   ├── Sphere.cpp
│   │   └── Sphere.h
│   ├── outputWriter
│   │   ├── OutputFacade.cpp
│   │   ├── OutputFacade.h
│   │   ├── VTKWriter.cpp
│   │   ├── VTKWriter.h
│   │   ├── XYZWriter.cpp
│   │   ├── XYZWriter.h
│   │   ├── vtk-unstructured.cpp
│   │   ├── vtk-unstructured.h
│   │   └── vtk-unstructured.xsd
│   ├── simulation
│   │   ├── ForceCalculation.cpp
│   │   ├── ForceCalculation.h
│   │   ├── GravitationalForce.cpp
│   │   ├── GravitationalForce.h
│   │   ├── LennardJonesForce.cpp
│   │   ├── LennardJonesForce.h
│   │   ├── Simulation.cpp
│   │   └── Simulation.h
│   ├── utils
│   │   ├── ArrayUtils.h
│   │   ├── Input.h
│   │   ├── Logger.h
│   │   ├── MaxwellBoltzmannDistribution.h
│   │   ├── PContainer.h
│   │   └── ParticleGenerator.h
│   └── xsd
│       ├── Simulation.cxx
│       ├── Simulation.hxx
│       └── Simulation.xsd
└── tests
    ├── CuboidInputReader_test.cc
    ├── DirectSumParticleContainer_test.cc
    ├── GetNeighbours_test.cc
    ├── LennardJonesForce_test.cc
    ├── LinkedCellParticleContainer_test.cc
    ├── ParticleCell_test.cc
    ├── Simulation.xml
    ├── SphereInputReader_test.cc
    ├── XMLInputReader_test.cc
    ├── eingabe-cuboid.txt
    ├── eingabe-sphere.txt
    └── main.cc
```
