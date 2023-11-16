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

#for usage type -h or no arguments, yields:
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
- Googletest is a C++ testing framework. We include `googletest` in the `libs` and add it as subdirectory in CMake.
- Mocks are useful in unit testing where is needed to test components in isolation. ASSERT and EXPECT are two primary macro types in GoogleTest for asserting conditions in tests. When an ASSERT macro fails, it immediately aborts the current function. When An EXPECT macro fails, it allows the test to continue. In general we use ASSERT when a failed condition invalidates the remainder of the test, and EXPECT when checking multiple conditions in a single test.
- To run the tests, one can first build and compile the project and then run ctest in the test directory. To do this try the instructions:
```
mkdir build && cd build
cmake .. -DBUILD_TEST=ON
make
cd tests && ctest
```
- within a `tests` folder in the root directoy, we added a simple unit test for the ParticleContainer and Tests for the Model, the Simulation, the ForceCalculation and the FileReader, which are components we created in the context of this worksheet.

### Task 2 Continuous Integration
- CI Pipeline enables automated testing, version control integration, building, running tests, quality checks all in one, thus is useful in team software development. 
- We create `workflows` in `github`, including library installation, building with GCC flag -fsanitize=address and tests checking to check on the status of every push and pull request. Now it's able to check the run of every change on the project by Actions section in the Github.


### Task 3 Logging 
- Log Functions are regular function calls used for logging which do not include information about the source code location. Logging Macro automatically capture and include additional context such as file name, function name, and line number, and is useful for more detailed logging, especially when debugging. Thus we should choose Logging Macro in our project. 
- We choose `spdlog` as our logging tool and include it in the `libs`, thus no extra installation step for the users is needed. We allow changing logging level by the instruction -l<String>, where string can either be "off", "trace", "debug", "info", "error" or "critical" which represent the level. 
- In the debug level certain logs will be written into a logs.txt file to keep the output organised and we are using regular function calls, when there is no quick access to the location needed.

### Task 4 Collision of two bodies
- With `FileReader` under `inputHandling`, we accept the initialization of the cuboid simulation with input file. The file format should be as follow:
```
cuboid:
    position:     {x,y,z}       # x,y,z are the coordinates of the lower left front-side corner in double
    velocity:     {x,y,z}       # x,y,z are the inital velocity vector of the whole cuboid
    (N1xN2xN3):   {n1,n2,n3}    # n1,n2,n3 are the number of particles per dimension(N1,N2 and N3 are the dimensions) in integer
    mass:         m             # m is the mass of particles in double
    mesh-width:   h             # h is the mesh width of particles in double
    sigma:        s             # s is sigma in double
    epsilon:      e             #epsilon be epsilon in double

(arbitrary amount of cuboids accoding to the above format)
    .
    .
```
- From now on all the forces are in the format of the Lennard-Jones potential. To still keep the force from last worksheet useable, we want to be able to switch between the new and the old force dynamically. A lightweight solution to this is a function parameter/ variable for the calculateF() function. This way any function that fulfills the function interface of taking two particles and returning three double values (3D Vector of forces between the two particles), can be given to calculateF() as Parameter. calculateF() will then calculate the force between all particles using this function. `forceSimpleGravitational()` and `forceLennJonesPotentialFunction()` in `ForceCalculations.h` return functions that can be used as parameter/variable for calculateF() (the function parameter is a variable of the Model class).
- To initialize the velocity of the particles, we are using the `MaxwellBoltzmannDistribution` under `utils`. This gives us a random velocity vector based on the given dimension and Brownian Motion average velocity, which will be summed with the initial velocity of the cuboid.
- We implemented the `CuboidGeneration` Class under `inputHandling`. It receives the data input from the filereader, generates corresponding cuboids and pass the particles to particleContainer which then allows the calculation of forces between particle pairs. 

### Simulation
- Using the parameters start_time = 0, end_time = 5 and delta_t = 0.0002 as well as cuboid-data.txt (can be found in the input folder of the Project) as input to the program, we get files that yield the following simulation in Paraview (using a glyph filter and color to visualize the force):

https://github.com/Grazvy/PSEMolDyn_GroupB/assets/70058620/b5a0927c-3dbe-430a-b68d-0b7d16bcf828

- This is a good example to see the Lennard-Jones potential, notice the two particles on the top left in the end of the simulation and how their color changes. Both of them turn white when the distance between each other is sigma, which shows the valley of the potential graph. You can also observe the particles pushing each other during the collison, through the red color indicating a higher force acting upon the particles.
- To get a better understanding of what is happening, one can use two other visualisations:

Another glyph filter that adds arrows in the direction of the forces acting on particles in a certain region. This yields:


https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/79d9a51e-7a8c-465a-b125-4eae3803ae84


- In the beginning, there are almost no forces, as all particles are colored blue and there are no arrows in any direction. This can be explained by the mesh-width of the cuboids. It is set to $h = \sigma \cdot 2^{\frac{1}{6}}$, therefore very close to $\sigma$. As we are using the Lennard-Jones-Potential, this means that $\frac{d}{dx} U_{ij}$ is close to 0. Therefore all forces $F_{ij}$ of two particles with distance $h$ are close to zero and only weak forces are present.
- The other observation is, that when the frist cuboid, which has a high velocity towards the second cuboid(the bigger one), collides with the second cuboid, there are suddenly strongly opposing forces in the area, where they hit each other. Then after the first cuboid advances further into the second cuboid, there are lot of opposing forces within the middle of the second cuboid. These forces point in every direction and are probably caused by the very short distances between particles. For distances smaller than $\sigma$, $\frac{d}{dx} U_{ij}$ is negative and rapidly decreasing. This leads to $F_{ij} = \frac{d}{dx} U_{ij} \cdot \frac{(x_j - x_i)}{\| x_j - x_i \|}$ pointing in the opposite direction of $x_j$, when looking from perspective of $x_i$. After the collision, the particles drift apart and slowy the forces are reducing, until there is an almost stable state again with some weak forces.

To get a different understanding of what is happenind a the moment of collision another visualization can be used.
Instead of the previous glyph filter, one can use a filter, that adds arrows in the direction of the velocities, that particles have in a certain region. The size of the arrows corresponds to the magnitude of the velocity.(the color is now representing the magnitude of velocity, not the force) This yields:

https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/3d355bc3-7048-445e-b1df-333da732ab98


- The first interesting observation is an effect that is similar to Newton's cradle, where the second bigger cuboid gets hit from the top and in the first moments the particles in the middle stay still and mostly the bottom row of particles are experiencing high velocities and are drifting away.
- In general one can see that the particles in the area of the collision have velocities in three main directions. These directions are below the bottom surface of the larger cuboid, as well as to the left and right of the smaller cuboid. All of them are areas with no or few particles and next to the centre of the collision. Whereas the outer left and the outer right side of the larger cuboid stay roughly in place, because the impact direction of the collision is vertical.

### Performance
- We tested the performance of our Code within a Linux environment on an AMD Ryzen 7 5700U without VTK output and logging enabled. We used gcc with the optimization level `-O2`. As input parameters we used the two cuboids from the worksheet `delta_t = 0.0002` , `end_time = 5` , `sigma = 1` and `epsilon = 5`. The simulation took 114 seconds when using the Optimization with Newton's third law and 122 seconds without the optimization. For now the difference is not too big, because we are still simulating with just a few particles(~ 380 pcs).

















