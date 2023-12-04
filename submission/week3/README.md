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
We used Codesynthesis and a Tree Mapping for parsing our XML files. The files for parsing were created with `xsdcxx cxx-tree --std c++11 parameters.xsd`, where `parameters.xsd` is our defined Schema (can be found in input/). When building, the `parameters.xsd` is copied into the build folder automatically to ensure correct validation against the Schema definiton, when reading XML files. An example of a correct XML file is provided in the input/ folder as well. In short,`outputParameters` contains the `baseName` for the .vtu output files and the `writeFrequency` with which output file will be written. `simulationParameters` containes `tEnd`, which is the end time of the simulation and `deltaT`, which is the step size for the simulation. Additionaly `cutOfRadius`, which is the max distance at which forces will be calculated, meaning if two particles are further apart than `cutOfRadius`, no forces will be calculated between them. Then the `cellSize` that corresponds to the size of the cells in which the domain will be divided and the type of `boundaryConditions`. Lastly the `domainDimensions` can be specified in `simulationParameters`. After that arbitrarily many `cuboids` and then arbitrarily many `spheres` can be specified

### Task 2 Linked-cell algorithm
- TO-DO

### Task 3 Boundary conditions
As asked in Taks 2 we implemented a BoundaryIterator to iterate over all cells that are close to the boundary. The depth of the iteration, so how much cells deep, starting from outer the boundary, the Iterator iterates is variable. For the actual Boundary Conditions, we used a different approach. We iterate over every side of the cuboid that is the domain and apply Boundary Conditions using Ghost Particles. This simply means that we look at eaach of the surfaces e.g. the top of the cuboid that represents the domain and then iterate over all Particles, that are closer than $\sigma \times 2^{\frac{1}{6}}$. For each of these Particles, we then "mirror" the position of the particle at the respective boundary (so if iterating over the top surface, mirror at the top boundary) and then calculate the force between this mirrored Particle and the actual Particle. The Ghost Particles are not actually created though.  Applying this to the given falling frop scenarion, yields:


https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/74f89c7e-2574-4863-be13-e8a8a3f21b88



### Task 4 Simulation of a falling drop - Wall”
- TO-DO

### Simulation
- TO-DO

### Performance
- TO-DO
















