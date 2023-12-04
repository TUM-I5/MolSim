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
- TO-DO

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
















