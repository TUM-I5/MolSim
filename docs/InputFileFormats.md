<!-- markdownlint-disable-next-line -->
@~english
\page InputFileFormats Input File Formats

## Overview

Our program supports multiple file formats as input files, which are interpreted differently. The file format is automatically determined by the file extension.
The following file extensions are supported:

- `.ps`: Contains Raw Particle Data
- `.cub`: Contains arguments for the internal cuboid generator
- `.xml`: Contains data for particle generation and allows to specify all the input parameters for the simulation as well

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
  - Grid Dimensions: '`int` `int` `int`'
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

### .xml

The `.xml` file format can be used to specify all the input parameters for the simulation at once. Therefore it is preferred over the other file formats.

Its definition is based on the [simulation_schema.xsd](simulation_schema.xsd) file, which is used to validate the input file. The file contains a single root element `<configuration>` with the following child elements:

- Simulation Settings:
  - `<fps>`: The number of frames per second to be rendered
  - `<video_length>`: The length of the video in seconds
  - `<delta_t>`: The time step size
  - `<end_time>`: The time at which the simulation should end
  - `<particle_container>` Which particle container implementation should be used
  
- Particle Data:
  - `<cuboid_spawner>`: Input data for a cuboid spawner, which generates particles in a cuboid shape
  - `<sphere_spawner>`: Input data for a sphere spawner, which generates particles in a spherical shape
  - `<individual_particle>`: Input data for a single particle, which is placed at the specified position

An example file could look like this:

```xml
<?xml version="1.0"?>
<configuration xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:noNamespaceSchemaLocation="simulation_schema.xsd">

    <settings>
        <fps>24</fps>
        <video_length>10</video_length>
        <delta_t>0.014</delta_t>
        <end_time>10.0</end_time>
        <particle_container>
            <directsum_container />
        </particle_container>
    </settings>

    <particles>
        <cuboid_spawner>
            <lower_left_front_corner>
                <x>0.0</x>
                <y>0.0</y>
                <z>0.0</z>
            </lower_left_front_corner>
            <grid_dim>
                <x>40</x>
                <y>8</y>
                <z>1</z>
            </grid_dim>
            <grid_spacing>1.1225</grid_spacing>
            <temperature>0.1</temperature>          <!-- aka mean brownian motion -->
            <mass>1</mass>
            <velocity>
                <x>0.0</x>
                <y>0.0</y>
                <z>0.0</z>
            </velocity>
            <type>0</type>
        </cuboid_spawner>
    </particles>

</configuration>
```
