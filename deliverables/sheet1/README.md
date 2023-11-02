# PSE Molecular Dynamics WS23/24 - Group F

## Members
- Alp Kaan Aksu
- Berke Saylan
- Feryal Ezgi Aşkın

## Code
- Link:     https://github.com/alpkaanaksu/MolSim
- Branch:   master

**Works with:**
- **Compiler:** clang 15.0.0
- CMake 3.27.7
- GNU Make 3.81

*Other compilers / versions would probably also work but we only tested with these.*


## Compiling and running the program

- You need `xerces-c` and `boost` (`Boost.program_options`) to compile the program.

```bash
mkdir build
cd build
```

```bash
ccmake ..
```

```bash
make
```

The default output file path for simulation files is `output/`. Make sure there is a folder called `output/` relative to where you are (`build/` in this case), else it does not work.

```bash
mkdir output
```

```bash
./MolSim ../input/eingabe-sonne.txt
```

The last line starts the program to run the simulation specified in `input/eingabe-sonnte.txt` with some sensible defaults.

---

`./MolSim --help` shows you all possible CLI arguments.

### Doxygen

```bash
make doc_doxygen
```

# Report: First steps towards a molecular dynamics simulation

## First Steps  ##

First we forked the repository [MolSim](https://github.com/TUM-I5/MolSim) and setup the environment to visualize the output of our simulation, which is [Paraview](http://paraview.org/). Next we modified the CMakeLists.txt by linking Xerces-C to our project. Then we had a problem with xsd when compiling using C++17. As a workaround we opted for downgrading the standard to C++14.

## Completion of the program frame ##
In this task, we have to complete the program frame to create a physics model for given particles:

To simulate particle interactions and movements over time, we make use of a simple force calculation, based on the gravitational potential energy of each particle, and Strömer Verlet for their updated positions and velocities.

The method `basicModel(double deltaT)` in `Model` achieves this, in which `deltaT` is the time step that is taken.

In the first lecture it was mentioned that in future worksheets other potentially more precise or efficient formulas for the calculation of physical phenomena like velocity, force etc. might be needed than the ones used so far, so we wanted to encapsulate all formulas in a separate class called `Model`.

For this we defined three attributes in the `Model` class: force, position, and velocity, which are placeholders for functions that match the specified function signature. Specifically, the velocity and position attributes are designed to accept a single Particle object reference as a parameter, while the force attribute accepts two `Particle` object references. These attributes should allow different formula implementations for the calculation of force, position and velocity of particles within the simulated physical system.

`basicModel` function takes the delta time as a parameter, which is subsequently utilized in the calculations. Inside the `basicModel` function, the force, position, and velocity calculation functions are defined according to the basic formulas given in the first lecture using lambda expressions and then set as the current attributes of the Model class. These formulas use the properties of the Particle class, such as mass, (old) position, (old) velocity, and (old) force, to compute the respective force, position, and velocity updates for the particles.

By encapsulating these formulas as attributes within the Model class, we wanted to create a flexible and extensible framework that could be easily adapted to different simulation scenarios and formula variants. This abstraction also allows other classes/interfaces in the program to interact with the Model class without needing to understand the underlying implementation details, promoting a cleaner and more maintainable source code.

---

The next challenge was to visualize and document the changes upon all particles after each time step. For this purpose we implemented the `outputWriter` directory and the abstract `Writer` class. This class provides a single function `plotParticles` that formats the particle data. This output format can be extended during future implementation and makes it easier to integrate new output formats on top of `VTK` and `XYZ`. The `VTKWriter` class is responsible for writing particle simulation data to VTK files. This is a format used by tools like ParaView that helps simulate three dimensional data.

---

The main entry point to our program is `MolSim.cpp`. The changes to this class include but aren't limited to the seperation of the program's functionality and execution. The program behaviour is mainly specified under `models`: this is where the force, velocity and position calculations take place. In `MolSim.cpp` the desired values can be passed as command-line arguments such as input file path, output file path, video duration, frames per second, end time, time step, and output type (VTK or XYZ).

We used `Boost.program_options`to parse command line arguments.

`MolSim.cpp` configures and creates a `Simulation` object with the passed arguments and runs it. `Simulation.cpp` is where our program gets assembled and executed. The `run` method continuously modifies particle characteristics and saves this particle data in the appropriate output format (VTK or XYZ). Moreover, the class provides a user-friendly string representation of its parameters and supports easy output to a stream using the << operator.

Rendering videos in ParaView was a challenge on its own at the beginning. The program was too slow, exporting took around 40 minutes and the resulting video was several minutes long (and the resulting animation was pretty bad, not smooth at all.). After playing around with our program and ParaView, we realized that we could fix the problem by reducing the number of exported states. We calculate the interval for outputs dynamically by defining a frame rate and video duration in our program (we have command line arguments for this, the default is 60 seconds with 24 fps). We can control the duration and frame rate very easily, using ParaView also got much more pleasant.

## Simulation of Halley’s Comet ##
Halley's Comet is one of the most famous celestial bodies in history. With it's orbital period of around 76 years and next perihelion on July 28, 2061, for most it is a once in a lifetime sight. At this time, it is almost the furthest away from us just beyond the distance of the orbit of Neptune.

For our simulation we took the default parameters as t<sub>end</sub> = 1000,  Δt = 0.014 as given in the problem statement. We also set the video duration to be 60 seconds with 24 frames per second. We then visualized the output of our simulation in ParaView, applying a glyph filter to represent particles as appropriately scaled spheres. Here's our result:



https://github.com/alpkaanaksu/MolSim/assets/101501911/a2dd9979-d36d-4d70-9f6f-765bab6861d3



In this video, the particles are color-coded according to their relative velocity magnitude. Even at the first glance, one can tell that the dark blue particle represents the Sun, as it has the smallest velocity and other particles orbit around it. The red particle is the Earth wheras the white one is Jupiter, because of their distance to Sun and orbital period. According to [NASA](https://science.nasa.gov/jupiter/facts/) "Jupiter makes a complete orbit around the Sun in about 12 Earth years". If you observe the first 5 second of our simulation, in which Jupiter completes its first full orbit, the Earth completes 12 turns around the Sun.

The particle with an elliptic orbit, which changes color throughout its movement is clearly Halley's Comet. It accelerates when approching other particles due to their masses and Jupiter does approximately 6 turns around the sun for every turn Halley's does. 6 times 12 is 72, which is roughly equal to the comet's orbital period in this simulation.

Inspecting `eingabe-sonne.txt` gives us additional insight into the masses of the particles. The scientific notation $3.0e-6$ converted into decimal notation is $3.0 \cdot 10^{-6} = 0.000003$. [The solar mass is about 333000 times the mass of earth](https://en.wikipedia.org/wiki/Solar_mass) and, the first particle's mass divided by the second's equals approximately this number. A similar calculation can be done for the first and third particles, which are thought to be the  <span style="color:#FF5733">Sun</span> and <span style="color:#52C74B"> Jupiter</span>: <span style="color:#FF5733">1</span> / <span style="color:#52C74B">0.000955</span> $\approx$ 1047.

Thus, our observations from the video can be confirmed.

## Refactoring and documentation ##
For the refactoring of the code, a new class named `ParticleContainer` was needed to encapsulate a collection of particles, which then allows us to perform various operations on all particles within the container. This approach simplifies the management and manipulation of particles by providing a centralized and structured way to handle them.

We decided to use the vector data structure for storing particles, because current version of the simulation doesn’t require us to insert/delete many particles during runtime, however it may still be required in future worksheets, therefore limited dynamic resizing capability vectors is good enough. Moreover vector enables more efficient sequential access to the particles than a list, which might be useful in future simulations.

We then implemented the `applyToAll` and `applyToAllPairs` methods, which make use of Iterator and Strategy pattern ideas. The `applyToAll` method has a function as a parameter, which expects a particle object reference as a parameter, and iterates through all the particles in the conteiner to apply the function to each of them. `applyToAllPairs` is analogue to `applyToAll` method, with the only difference that here parameter function expects 2 particle object references in its method signature. By utilizing these functions we can do the velocity, force etc. calculations on all particles in the `Model` class. We also defined basic data structure functions such as add, delete, count, get etc. for potential future use.

---

We also created a base class called `Writer` for all output operations. `XYZWriter` and `VTKWriter` both derive from this class. This implementation of strategy pattern allows us to leave choosing the specific output time to the runtime. 