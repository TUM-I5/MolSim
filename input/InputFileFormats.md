<!-- markdownlint-disable-next-line -->
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

```text
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

- Consists of one or more definitions of cuboids with the following arguments (each on a seperate line), where the values on each line are seperated by spaces:
  - The coordinate of the lower left front-side corner: '`double` `double` `double`'
  - Grid Dimensions: '`size_t` `size_t` `size_t`'
  - Distance of the particles (mesh width): '`double`'
  - Mass of a single particle: '`double`'
  - Initial velocity of the particles: '`double` `double` `double`'
  - Mean-value of the velocity of Brownian Motion (for temperature dependent wobbling) '`double`'
  - Type of the cuboid: '`int` used for labeling particles in the cuboid'
- Outside the cuboid definitions an arbitrary amount of comment lines, which start with `#` and empty lines are allowed, furthermore comments are allowed at the end of a line after the last argument

Note: Floating point numbers can be written in scientific notation, e.g. `1.0e-3` for `0.001`.

An example file could look like this:

```text
# Definition of Cuboid 1
0.0 0.0 0.0       # Lower left front-side corner
10 10 10          # Grid Dimensions
0.1               # Distance of the particles (mesh width)
1.0               # Mass of a single particle
0.4 0.0 0.0       # Initial velocity of the particles
0.0               # Mean-value of the velocity of Brownian Motion
0                 # Type of the cuboid

# Definition of Cuboid 2
10.0 10.0 10.0    # Lower left front-side corner
10 10 10          # Grid Dimensions
0.1               # Distance of the particles (mesh width)
1.0               # Mass of a single particle
0.0 0.4 0.0       # Initial velocity of the particles
0.0               # Mean-value of the velocity of Brownian Motion
1                 # Type of the cuboid
```
