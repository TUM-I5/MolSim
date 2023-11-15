# Group B
## Project Information

**Members:**
    Yuchen Zhao,
    Grazvydas Kuras,
    David Kratz

[Project Link](https://github.com/Grazvy/PSEMolDyn_GroupB)

**Last commit:** commit-id:   commit-msg: ""

**Build/Usage:**
```
mkdir build && cd build
ccmake ..

#for executable
make 
./MolSim [-e<double>] [-t<double>] [-l<String>] -f<String>
#-e<double>:        gives the end_time of the simulation
#-t<double>:        gives the step size used for the simulation
#-f<String>:        gives the filename from which the initial state of the Particles is read, these are the particles that will get simulated
#-l<String>:        specifies the level of logging, e.g. how fine grained programm logs are. String can either be "off", "trace", "debug", "info", "error" or "critical". The default level is "debug".

#for documentation
make doc_doxygen 
```

**Notes:**
Call ./MolSim with no arguments or the -h argument to get a help message about the 
command line arguments and what is being returned by the executable. This file should probably be viewed on GitHub, as some media embeddings might not work in e.g. an IDE 

## Report
### Task 1 GoogleTest
- Googletest is a C++ testing framework. We include `googletest in the `libs` and add it as subdirectory in Cmake.
- Mocks are useful in unit testing where is needed to test components in isolation. ASSERT and EXPECT are two primary macro types in GoogleTest for asserting conditions in tests. When an ASSERT macro fails, it immediately aborts the current function. When An EXPECT macro fails, it allows the test to continue. In general we use ASSERT when a failed condition invalidates the remainder of the test, and EXPECT when checking multiple conditions in a single test.
- To run the tests, one can first build and compile the project and then run ctest under the test directory. To do this try the instructions:
```
mkdir build && cd build
cmake .. -DBUILD_TEST=ON
make
cd tests && ctest
```
#Tests introduction#

### Task 2 Continuous Integration
- CI Pipeline enables automated testing, version control integration, building, running tests, quality checks all in one, thus is useful in team software development. 
- We create `workflows` in `github`, including library installation, building with GCC flag -fsanitize=address and tests checking to check on the status of every push and pull request. Now it's able to check the run of every change on the project by Actions section in the Github.


### Task 3 Logging 
- Log Functions are regular function calls used for logging which do not include information about the source code location. Logging Macro automatically capture and include additional context such as file name, function name, and line number, and is useful for more detailed logging, especially when debugging. Thus we should choose Logging Macro in our project. 
- We choose `spdlog` as our logging tool and include it in the `libs`, thus no extra installation step for the users is needed. We allow changing logging level by the instruction -l<String>, where string can either be "off", "trace", "debug", "info", "error" or "critical" which represent the level. 


### Task 4 Collision of two bodies
- With `FileReader` under `inputHandling`, we accept the initialization of the cuboid simulation with input file. The file format should be as follow:
```
"{"+x+","+y+","+z+"}"       # x,y,z be the coordinates of the lower left front-side corner in double
v                           # v be the Brownian Motion Velocity in double, witch will be the parameter of maxwell boltzmann distribution
"{"+N1+","+N2+","+N3+"}"    # N1,N2,N3 be the number of particles per dimension in integer
m                           # m be the mass of particles in double
h                           # h be the mesh width of particles in double
sigma                       # sigma be sigma in double
epsilon                     #epsilon be epsilon in double
```
- From now on all the forces are in the formation of Lennard-Jones potential. To still keep the force from last worksheet workable, we maintain the simple gravitation calculation. To do this we create an interface `ForceCalculation` under `utils`, which transmits two functions `forceSimpleGravitational` and `forceLennJonesPotentialFunction`(which requires `sigma` and `epsilon` as parameters). Different force calculation methods can be switched with `forceType` `LennJones` and `simple`.
- To initialize the velocity of the particles, we implement the `MaxwellBoltzmannDistribution` under `utils`. The function `maxwellBoltzmannDistributedVelocity` requires the input `averageVelocity` and `dimensions`, and can generate random velocities for particles that fit the maxwell boltzmann distribution. 
- We implement the `CuboidGeneration` Class under `inputHandling`. It receives the data input from the filereader, generates corresponding cuboids and pass the particles to particleContainer which then allows the calculation of forces between particle pairs. 


### OneMoreThing
- #Restructuring the project# , #calcutation in threads# etc






[^1]: https://en.wikipedia.org/wiki/List_of_gravitationally_rounded_objects_of_the_Solar_System
[^2]: https://en.wikipedia.org/wiki/Halle












