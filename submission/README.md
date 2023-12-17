# Group B
## Project Information

**Members:**
Yuchen Zhao,
Grazvydas Kuras,
David Kratz

[Project Link](https://github.com/Grazvy/PSEMolDyn_GroupB)

**Last commit:** commit-id: 4031c37 or "Merge remote-tracking branch 'origin/master'"

**Build/Usage:**
```
mkdir build && cd build
ccmake ..

#for executable
make

#for usage type -h or no arguments, yields:
Usage ./MolSim [-l<String>] [-p] [-o] -f<String>
 Info:              See the /input folder for the parameters.xsd schema, in which 
                    program arguments should be specified
 -f<String>:        gives the filename of an .xml file, that has to follow
                    the xsd schema defined in input/parameters.xsd.
                    from this file all programm arguments / options will be read(see README)
 -l<String>:        specifies the level of logging, e.g. how fine grained programm logs are.
                    can either be "off" "trace", "debug", "info", "error" or "critical".
                    The default level is "debug".
 -h                 prints a help message
 -p                 if the flag is set, the programm will measure the time for the execution.
                    therefore no vtk output and no logging will happen (specifing a log level at
                    the same time is undefined behaviour)

Returns:
                  several .vtu files that can be used for visualisation in Paraview

#for documentation
make doc_doxygen 
```

**Notes:**
Call ./MolSim with no arguments or the -h argument to get a help message about the
command line arguments and what is being returned by the executable. This file should probably be viewed on GitHub, as some media embeddings might not work in e.g. an IDE

## Report
### Task 1 Thermostats
- in Misc it is explained how to correctly specify the Thermostats in the XML input file.
  For every iteration of the Thermostats, first the kinetic energy of the current system is calculated according to the formula, then the scaling factor $\beta$ is calculated and the absolute size of $\beta$ is capped by $\delta T$ / the maximal temperature difference given in the XML input file. 


### Task 2 Simulation of the Rayleigh-Taylor instability


### Task 3 Simulation of a falling drop - Liquid
- the `Checkpointer` is a seperate component, that serializes all the particles that are currently 
  in our `CellContainer` and prints them into a file. For every particle, the respective sigma and epsilon is stored as well. A file of this format can be deserialized into a list of (particle,sigma,epsilon) tuples and these can bes added into a `CellContainer`



### Task 4 Simulation of a falling drop - Wall”




### Performance



### Misc
#### XMl File
- the new file format includes a optional `Thermostats` component in the `simulationParameters`. 
  In `Thermostats`, one has to specify the initial temperature of the system and the frequency, with
  which the Thermostats is applied. A target temperature and the maximal temperature difference are optional. The maximum temperature difference sets the limit for the magnitude of a temperature update from the current temperature towards the target temperature. The `outputParameters` now contain an option for a checkpoint file, that should be used as input to the next simulation and an option that will produce a checkpoint file at the end of the simulation. The checkpoint input file has to be a file that was produced by our program. The sigma and epsilon in the cuboid/sphere component of the Schema are now applied to the cuboids in the simulation and it is possible to specify a `meanVelocity` in the cuboids/spheres component. Setting a `meanVelocity` and Thermostats at the same time does not make sense and is undefined behaviour. The `boundaryConditions` (in the different directions) can now either be 'outflow', 'reflective' or 'periodic'.
#### Boundary Conditions















