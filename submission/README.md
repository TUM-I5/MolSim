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
command line arguments and what is being returned by the executable. This file should probably be viewed on GitHub, as some media embeddings might not work in e.g. an IDE. Sadly GitHub only allows to upload videos with less than 10 Mb, therefore most of the videos have a mediocre quality and are relatively short.

## Report
### Task 1 Thermostats
- in Misc it is explained how to correctly specify the Thermostats in the XML input file.
  For every iteration of the Thermostats, first the kinetic energy of the current system is calculated according to the formula, then the scaling factor $\beta$ is calculated and the absolute size of $\beta$ is capped by $\delta T$ / the maximal temperature difference given in the XML input file. 


### Task 2 Simulation of the Rayleigh-Taylor instability


### Task 3 Simulation of a falling drop - Liquid
- the `Checkpointer` is a seperate component, that serializes all the particles that are currently 
  in our `CellContainer` and prints them into a file. For every particle, the respective sigma and epsilon is stored as well. A file of this format can be deserialized into a list of (particle,sigma,epsilon) tuples and these can be added into a `CellContainer`
- when running the simulation of the falling drop with the given parameters and an equlibrated fluid at the bottom, we get the following  simulation.  The color shows the amount of velocitiy that particles have.


https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/2c07ac5f-6ec8-4850-83f6-b1184dc499c5

the same simulation with arrows, which have corresponding amount and direction to the velocity of the particles

https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/c331437e-ccae-4806-b990-a1dab352802c

There are different observations, that can be made. In the initial moment when the drop hits the surface of the liquid, the velocity (and force) spreads like a shockwave in a cricle around the point of impact.


![shock_wave](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/477d60d3-911d-4b9d-9548-a2b7c83f8571)


Once the lower part of the "shockwave" reaches the bottom, it is reflected due to the reflecting boundaries.

![reflection_marked](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/32b1a56c-469f-4c40-9c10-074cd1743fd5)


After that a steady wave is running from the middle outwards and we can see that the lower part of the wave is spreading faster than the upper part.

![final_wave_marked](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/b3c86025-9c3a-4b0e-88a8-0b444d8b32c5)


When the wave, that steadily moves outwards, hits the left/ right boundary, the wave is "breaking". This means the particles are moving upwards, because there initial movement goes outwards, but due to the boundaries, they can not move further into that direction and then have to move upwards.

![short_before_break_marked](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/bf7e8137-9fa0-4554-b747-9385affc4b9e)

![wave_breaking_at_border_marked](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/4c2851f1-12ad-4e8f-b205-da9e1a312452)

At the same time, some of the upper particles of the drop are moving outwards with very high velocity, above the liquid.

![fast_particles_marked](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/0e44da09-60cf-44c6-8fbe-8a46f3211e5a)

After some time the gravity is showing it's effect on these particles and they are, stil with high velocity, moving downwards again.

![final_particles2_marked](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/843e2ec4-cdbe-4af7-bccb-308ead773b86)

At the same time, the previously described steady wave is breaking at the left and right boundary and therefore, the two groups of particles are "crashing"

![short_before_break_marked](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/f0933cb6-8c37-44a7-b195-a12e891986f2)


![before_the_crash_marked](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/8b29b571-55cb-4fa5-a61a-8c6ff77e0e33)


Lastly we observed, that after the initial displacement of the fluid from the middle, the reflected waves are meeting in the middle again. (This can be seen best without any coloring)

After the drop hits the water, t ~ 8.25, the particles are distributed like this:

![crater](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/7e4b79b1-0b56-4c88-ad98-93c38e17a970)

After the waves were reflected, t ~ 17.5, the particles are distributed like this:

![inv_crater](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/d7198861-f8e8-4558-b528-72ad6217b468)

To sum up, the simulation has some expected physical properties, like the waves, that are moving outwards from the point of impact. These waves are reflected by the boundary then. Apart from that there are a few particles that are "splashing" away with high speed. This looks similar to videos of a real droplet into fluid (e.g. https://www.youtube.com/watch?v=cNI-LIVs-to ). Altough one thing is missing in our simulation, namely the drop jumping back upwards right after the initial collision with the fluid. This might be missing in our simulation due to the fluid being not very deep.

### Performance



### Misc
#### XMl File
- the new file format includes a optional `Thermostats` component in the `simulationParameters`. 
  In `Thermostats`, one has to specify the initial temperature of the system and the frequency, with
  which the Thermostats is applied. A target temperature and the maximal temperature difference are optional. The maximum temperature difference sets the limit for the magnitude of a temperature update from the current temperature towards the target temperature. The `outputParameters` now contain an option for a checkpoint file, that should be used as input to the next simulation and an option that will produce a checkpoint file at the end of the simulation. The checkpoint input file has to be a file that was produced by our program. The sigma and epsilon in the cuboid/sphere component of the Schema are now applied to the cuboids in the simulation and it is possible to specify a `meanVelocity` in the cuboids/spheres component. Setting a `meanVelocity` and Thermostats at the same time does not make sense and is undefined behaviour. The `boundaryConditions` (in the different directions) can now either be 'outflow', 'reflective' or 'periodic'.
#### Boundary Conditions















