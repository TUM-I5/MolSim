MolSim PSEMolDyn_GroupG
=======================

The code for the practical course Molecular Dynamics of group G.

## Build and run: 

### Build
1. Navigate into the build folder: `cd build` 
2. Run `cmake ..` to create the makefile. We use clang as the compiler 
3. Run `make` to generate the executable and `make doxygen`if you want to generate the doxygen documentation

### Run 
After building the project you can run the executable 
1. Navigate into the build folder: `cd build` 
2. If you run the program for the first time, you need to create an **outputVTK** and **outputXYZ** folder in the build folder: `mkdir outputVTK`, `mkdir outputXYZ`
3. Call ./MolSim input end_time delta_t where **input** is the (relative) path to the input file, **end_time** is the end time of the simulation, and **delta_t** is the time increament
4. The generated files can be found at ~/build/output

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