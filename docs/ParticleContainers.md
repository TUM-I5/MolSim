<!-- markdownlint-disable-next-line -->
\page ParticleContainers Particle Containers

## Overview

Currently there exist two different particle container implementations:

- DirectSumContainer: A naive implementation, which calculates the forces between all particles
- LinkedCellsContainer: A more efficient implementation, which uses the linked cells algorithm to calculate the forces between particles

## DirectSumContainer

The DirectSumContainer is a naive implementation of a particle container, which calculates the forces between all particles. It calculates the forces between all particles in the container and all particles in the simulation. This is the simplest implementation, but also the slowest.

It has a runtime complexity of `O(n^2)`, where `n` is the number of particles in the container.

## LinkedCellsContainer

The LinkedCellsContainer is a more efficient implementation of a particle container, which uses the linked cells algorithm to calculate the forces between particles. It calculates the forces between the particles by only considering the particles in nearby cells. This is a lot faster than the DirectSumContainer, since only a small fraction of all particles has to be considered.

It has a runtime complexity of aproximately `O(n)`, where `n` is the number of particles in the container.

## Comparison of the two implementations

The following graphs show the runtime of the two implementations for different numbers of particles. The runtime is measured in seconds.

It is interesting to note, that it can be seen quite clearly, that the runtime of the DirectSumContainer increases quadratically with the number of particles, while the runtime of the LinkedCellsContainer increases linearly.

Additionally the benchmark was about twice as fast on an Intel Core i5-12600 CPU than on an AMD Ryzen 5 3600 CPU.

### AMD Ryzen 5 3600

The first benchmark was run on an AMD Ryzen 5 3600 CPU and 16 GB of RAM.

\image html BenchmarkAMD.png width=720px

### Intel Core i5-12600

The second benchmark was run on an Intel Core i5-12600 CPU and 16 GB of RAM.

\image html BenchmarkIntel.png width=720px
