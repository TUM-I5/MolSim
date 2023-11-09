\page InputFileFormats Input File Formats

## Overview

Our program supports multiple file formats as input files, which are interpreted differently. The file format is automatically determined by the file extension. 
The following file extensions are supported:
- `.ps`: Contains Raw Particle Data
- `.cub`: Contains arguments for the internal cuboid generator

In the following sections, the file formats are explained in detail.

## File Formats Explained

### .ps

The `.ps` file format contains raw particle data. It is used to directly specify the initial state of the simulation.

- Starts with an arbitrary amount of comment lines, which start with `#` and are only allowed at the beginning of the file
- The first non-comment line contains a single integer, the number of particles in the simulation
- Now follow an amount of lines equal to the number of particles, each containing the following arguments (each seperated by spaces):
  - The particle's position in 3D space (Three floating point numbers, seperated by spaces)
  - The particle's initial velocity (Three floating point numbers, seperated by spaces)
  - The particle's mass (A single floating point number the particle's mass)

Note: Floating point numbers can be written in scientific notation, e.g. `1.0e-3` for `0.001`.

An example file could look like this:

```
# This is a comment
# xyz-coord      velocity        mass
4
0.0 0.0 0.0      0.0 0.0 0.0     1.0
0.0 1.0 0.0     -1.0 0.0 0.0     3.0e-6
0.0 5.36 0.0    -0.425 0.0 0.0   9.55e-4
34.75 0.0 0.0    0.0 0.0296 0.0  1.0e-14
```

### .cub

The `.cub` file format contains arguments for the internal cuboid generator. The program uses these arguments to generate one or multiple cuboids of particles given the specified parameters. 

- Consists of one ore more definitions of cuboids with the following arguments (each on a seperate line), where the values on each line are seperated by spaces:
  - The coordinate of the lower left front-side corner: '`double` `double` `double`'
  - Number of particles per dimension: '`int` `int` `int`'
  - Distance of the particles (mesh width): '`double`'
  - Mass of a single particle: '`double`'
  - Initial velocity of the particles: '`double` `double` `double`'
  - Mean-value of the velocity of Brownian Motion (for temperature dependent wobbling) '`double`'
- Outside the cuboid definitions an arbitrary amount of comment lines, which start with `#` and empty lines are allowed

Note: Floating point numbers can be written in scientific notation, e.g. `1.0e-3` for `0.001`.

An example file could look like this:

```
# Definition of Cuboid 1 follows

# Lower left front-side corner
# Number of particles per dimension
# Distance of the particles (mesh width)
# Mass of a single particle
# Initial velocity of the particles
# Mean-value of the velocity of Brownian Motion
0.0 0.0 0.0
10 10 10
0.1
1.0
0.4 0.0 0.0
0.0

# This is another comment
# Definition of Cuboid 2 follows
0.0 0.0 0.0
20 20 30
0.05
0.3
-0.4 0.1 0.0
0.1
```
