# Group B
## Project Information

**Members:**
Yuchen Zhao,
Grazvydas Kuras,
David Kratz

[Project Link](https://github.com/Grazvy/PSEMolDyn_GroupB)

**Last commit:** commit-id: TO-DO

**Build/Usage:**
```
TO-DO
```

**Notes:**
Call ./MolSim with no arguments or the -h argument to get a help message about the
command line arguments and what is being returned by the executable. This file should probably be viewed on GitHub, as some media embeddings might not work in e.g. an IDE

## Report
### Task 1 XML input
-  We used Codesynthesis and a Tree Mapping for parsing our XML files. The files for parsing were created
   with `xsdcxx cxx-tree --std  c++11 parameters.xsd`, where `parameters.xsd` is our defined Schema (can be found in input/).
   When building, the `parameters.xsd` is copied into the build folder automatically to ensure correct validation
   against the Schema definiton, when reading XML files. An   example of a correct XML file is provided in the input/ folder as well.
-  In short,`outputParameters` contains the `baseName` for the .vtu output files and the `writeFrequency` with which output files
   will be written. `simulationParameters` containes `tEnd`, which is the end time of the simulation and `deltaT`, which is the step
   size for the simulation. Additionaly `cutOfRadius`, which is the max distance at which forces will be calculated, meaning if two
   particles are further apart than `cutOfRadius`, no forces will be calculated between them. Then the `cellSize` that corresponds to
   the size of the cells in which the domain will be divided and the type of `boundaryConditions`. Lastly the `domainDimensions` can be
   specified in `simulationParameters`. After that arbitrarily many `cuboids` and then arbitrarily many `spheres` can be specified

### Task 2 Linked-cell algorithm

- We implemented a CellContainer & a CellCalculator class similar to the previous ParticleContainer and Model,
  responsible for the linked cell force calculation algorithm and updating the velocity, position and distribution
  within the cell structure. The CellContainer contains the initialized particles and a distribution of pointers
  to those in a vector matrix with a vector<Particle *> as a cell. In the beginning we tried to use std::unordered_set<Particle *>
  but after some effort we realized that you cannot modify the values stored within. The CellContainer is also responsible
  for providing the iterations over the cells and so-called paths, again inspired by the consumer producer pattern for
  parallelization friendliness in week 5. 

- The linked cell algorithm is generally build such that multiple threads can update the particles without using locks.
  This is achieved by dividing each set of neighbours, with distance r_cutoff for all cells, into the sub problem of 
  comparing the center cell with just one of the neighbours. We are calling the direction from center to a specific cell
  a pattern. This pattern can be walked through the matrix and with specific starting points you can cover the whole 
  matrix with these non overlapping paths. So for each pattern you can run multiple paths simultaneously, but more about
  that hopefully in week 5. So with each step we are getting 2 cells within our cutoff distance, which will be combined
  for our force calculations using newtons 3rd law of course.

- In the next step we are iterating over all cells to finish the force calculations within, to calculate the velocity and
  the position of the particles inside, again dividable over multiple threads, where each cell can be calculated independently
  since the impact of all neighbour cells is already applied. Notice that this order is not the same as the one used with
  ParticleContainer. Previously, we initialized F and then iteratively calculated X F V. Now, the order is shifted, so we
  are initializing F X and then iteratively calculating F between neighbours, finishing F calculations and calculating V X
  within each cell, so FVX instead. The advantage is first up a more elegant way of implementing our algorithm, because
  in order to finish F, calculate X and V we use the same cell. Alternatively we would need to iterate for X and ~F V
  separately, due to the linked cell algorithm in between. This results in another necessary synchronisation step between
  threads, because you can't calculate V and X without the updated F. Besides the formulas, we checked the equality of 
  those to orders in the CellCalculator tests under "test_newOrder".

- dynamic cell size, theory and benchmarks

- cutoff left

- halo cells

- simulation of two bodies

- performance new vs old

### Task 3 Boundary conditions
- As asked in Taks 2 we implemented a BoundaryIterator to iterate over all cells that are close to the boundary.
  The depth of the iteration, so how much cells deep, starting from outer the boundary, the Iterator iterates is variable.
- For the actual Boundary Conditions, we used a different approach. We iterate over every side of the cuboid that is the domain
  and apply Boundary Conditions using Ghost Particles. This simply means that we look at each of the surfaces e.g. the top of the
  cuboid that represents the domain and then iterate over all Particles, that are closer than $\sigma \times 2^{\frac{1}{6}}$.
  For each of these Particles, we then "mirror" the position of the particle at the corresponding boundary (so if iterating over the top
  surface, mirror at the top boundary) and then calculate the force between this mirrored Particle and the actual Particle.
  The Ghost Particles are not actually created though. This approach does not require to differentiate between particles at the
  corner or particles next to two boundaries, these cases are handled implicitly.
  Applying this to the given falling drop scenarion, yields:


https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/74f89c7e-2574-4863-be13-e8a8a3f21b88

- One can see the strongly opposing forces when hitting the boundary. It looks similar to crashing into another cuboid,
  which seems to fit the idea of Ghost Particles. Of course for further validation test are needed.

### Task 4 Simulation of a falling drop - Wall”



https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/9b02f47a-9632-4c42-81d8-06b821567c87




### Performance

- We compared our new implementation, that uses a cell based datastructure and the linked cell algorithm to our old implementation.
  We measured the time it takes to simulate a cuboid of  250, 500 , 1000 , 2000 ... , 16000 particles for the both implementations
  (we tried to avoid collisions with a boundary for the new implementation) and measured runtime and memory access:

<img src="https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/c9d9b698-5807-4c52-8f97-0237db0f230e" width="440">

<img src="https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/96ddfc1d-656e-4801-bb64-af2276e961b8" width="400">

- The plot justifies the assumptions, that while the old implementation has a complexity of $O(n^2)$, the new implementation
  has a linear complexity of $O(n)$. As example take the runtime for $2 \cdot 10^3$ and $4 \cdot 10^3$ particles, in the new
  implementation, the execution time increases from ~ 14 seconds to ~ 30 seconds, meaning like the input values, the runtime doubled.
  In the old implementation, the runtime went from ~ 75 seconds to ~ 243 seconds, meaning roughly a quadrupling of the output,
  which fits the $O(n^2)$ complexity, as $(2 \cdot n)^2 = 4 \cdot n^2$.
- When looking at the memory access of the program, we can see a similar behaviour, altough the difference between the two
  implementations is not as significant here. A reason for this could be, that while the linked cell algorithm does not have quadratic
  complexity, it has some additionaly complexity that requires memory accesses, like the moving of particles between cells (if they
  were updated, but now are in a wrong cell), which in some simulations and with a small cell size can happen very frequently.













