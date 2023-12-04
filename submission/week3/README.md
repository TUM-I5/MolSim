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
- TO-DO

### Task 4 Simulation of a falling drop - Wall”
- TO-DO

### Performance
- TO-DO
















