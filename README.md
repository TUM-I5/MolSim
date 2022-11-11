MolSim PSEMolDyn_GroupG
=======================

The code for the practical course Molecular Dynamics of group G.

Members: <br />
Alexandra Marquardt <br />
Matteo Wohlrapp <br />
Michael Borisov <br />
Group Id: G

Link to the repository: https://github.com/SecTedd/PSEMolDyn_GroupG <br />
Branch: assignement1 <br />
Commit Id: # 


Video: <br />
The stationary "particle" represents the sun, the celestial body with the shortest orbital peroid represents earth, the one with the second shortest orbital period represents Jupiter and the one with the longest orbital peroid represents Halley's Comet.


## Build and run: 

### Build without Doxygen
1. Navigate into the build folder: `cd build` 
2. Run `cmake ..` to create the makefile. We use gcc 11.2.0 as the compiler.
3. Run `make` to generate the executable.

### Build with Doxygen
1. Navigate into the build folder: `cd build`
2. Run `cmake -D BUILD_DOC=ON ..` to create the makefile. We use gcc 11.2.0 as the compiler.
3. Run `make` to generate the executable and `make doc_doxygen` to generate the documentation. 

### Run 
After building the project you can run the executable 
1. Navigate into the build folder: `cd build` 
2. Call `./MolSim input end_time delta_t` where **input** is the (relative) path to the input file (from the build folder), **end_time** is the end time of the simulation, and **delta_t** is the time increment
3. The generated files can be found at ~/build/outputXXX where **XXX** is the output type, e.g. VTK or XYZ
4. **Warning** the contents of the output folders will be overwritten in every run of the simulation!

## Logging: 
For logging we use spdlog. The logs are written to files which can be found in the **/logs** folder. The logs are separated into logic and memory logs. Logic logs are used to log events in the program flow. Within the logic logs, there is the distinction between input, output and simulation. Memory logs on the other hand document the construction and destruction of objects and therefore help to prevent memory leaks.

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
|-- logs
|-- README.md
`-- src
    |-- inputReader
    |   |-- FileReader.cpp
    |   |-- FileReader.h
        |-- InputReader.cpp
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