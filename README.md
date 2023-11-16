MolSim
===
# Group E #
Members:
* Layla
* Markus
* Sophy

# Code #
* Link:     https://github.com/sophykk/PSEMolDyn-GroupE.git
* Branch:   master
* Compiler: g++ 11.4.0
* Commit ID: 4f9e1fb
* Build and execute instructions:
 ```
 mkdir build
 cd build
 ccmake ..
 make
 ./MolSim ../input/task2.txt <5> <0.0002> (the first argument represents the end_time and the second argument represents the delta_t)
 
 Doxygen documentation: 
 make doc_doxygen

UnitTests:
ctest
 
 then for the visualisation go into ParaView and open the beforehand generated MD_vtk folder/group, hit apply and use the glyph filter with the according configurations on it
```
# Report #
## Task 1 ##

- added the configuration for the googletest in the CmakeLists.txt file
- implemented Unit tests, which are testing the default constructor, the constructor with a list as a parameter and the addParticle method of the ParticleContainer class.
- Can be executed by running ctest command. 

## Task 2 ##

- implemented the pipeline for CI, which checks on every pull_request and push to the master branch if the programm is compiling without errors and then executing the UnitTests automatically

## Task 3 ##

- added the configuration for the spdlog in the CmakeLists.txt file
- Logging: logging different key parts of our programm
- we decided to use spdlog functions instead of macros, because they allow for dynamic log level changes at runtime    using the 'spdlog::set_level(...)' function
- for consistency reasons we decided to use spdlog functions through our whole project

## Task 4 ##

- Collision of two bodies:
- a new ParticleGenerator class, which creates a grid (N1xN2xN3) of the partciles
- calculating LennardJones force in the Formulas class for the molecalas' intercation
- iterating throuigh ParticlePairs instead ParticleList for the force calculation
- new input file task2.txt with initiation values and the correspondingly updated FileReader with reading N values
- time measurement was not possible, because we didnt manage to get the correct output files for our simulation
