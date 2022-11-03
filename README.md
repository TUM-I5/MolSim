MolSim PSEMolDyn_GroupG
=======================

The code for the practical course Molecular Dynamics of group G.

Members: Alexandra Marquardt
         Matteo Wohlrapp
         Michael Borisov

Link to the repository: https://github.com/SecTedd/PSEMolDyn_GroupG
CommitId: XXXXXXXXXX


## Build and run: 

### Build without Doxygen
1. Navigate into the build folder: `cd build` 
2. Run `cmake ..` to create the makefile. We use g++ as the compiler.
3. Run `make` to generate the executable.

### Build with Doxygen
1. Navigate into the build folder: `cd build`
2. Run `cmake -D BUILD_DOC=ON ..` to create the makefile. We use g++ as the compiler.
3. Run `make` to generate the executable and `make doc_doxygen` to generate the documentation. 

### Run 
After building the project you can run the executable 
1. Navigate into the build folder: `cd build` 
2. Call `./MolSim input end_time delta_t` where **input** is the (relative) path to the input file (from the build folder), **end_time** is the end time of the simulation, and **delta_t** is the time increament
3. The generated files can be found at ~/build/outputXXX where **XXX** is the output type, e.g. VTK or XYZ

## Structure: 
```
.
|-- build
|-- cmake
|   `-- modules
|       `-- doxygen.cmake
|-- CMakeLists.txt
|-- Doxyfile
|-- input
|   `-- eingabe-sonne.txt
|-- libs
|-- README.md
`-- src
    |-- inputReader
    |   |-- FileReader.cpp
    |   |-- FileReader.h
    |   `-- InputReader.h
    |-- model
    |   |-- ParticleContainer.cpp
    |   |-- ParticleContainer.h
    |   |-- Particle.cpp
    |   `-- Particle.h
    |-- MolSim.cpp
    |-- outputWriter
    |   |-- OutputFacade.cpp
    |   |-- OutputFacade.h
    |   |-- vtk-unstructured.cpp
    |   |-- vtk-unstructured.h
    |   |-- vtk-unstructured.xsd
    |   |-- VTKWriter.cpp
    |   |-- VTKWriter.h
    |   |-- XYZWriter.cpp
    |   `-- XYZWriter.h
    |-- simulation
    |   |-- GravitySimulation.cpp
    |   |-- GravitySimulation.h
    |   |-- Simulation.cpp
    |   `-- Simulation.h
    `-- utils
        |-- ArrayUtils.h
        `-- MaxwellBoltzmannDistribution.h
```