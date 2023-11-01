# Group B
## Project Information

**Members:**
    ..
    ..
    David Kratz

[Project Link](https://github.com/Grazvy/PSEMolDyn_GroupB)

**Last commit:**

**Build/Usage:**
```
mkdir build && cd build
ccmake ..

#for executable
make 
./MolSim [-e end_time] [-t delta_t] -f file_name

#for documentation
make doc_doxygen 
```

**Notes:**
Call ./MolSim with no arguments or the -h argument to get a help message about the 
command line arguments and what is being  returned by the executable 

## Report

### Task 2
- Force, position and velocity were calculated according to the formulas in the lab meeting. For the Force calulation a simple optimization could be done such that only half of the inter-particle forces need to be calculated as $`F_{i,j} = - F_{j,i}`$
- To gain output that can be visualised by Paraview the `VTKWriter` Class was used. After a call to the executable with a correctly formatted file as input, one gets a group of .vtu files of the format `out_[iteration].vtu ` as output. These can be loaded into Paraview and one can start the simulation according to the tutorial in the lab meeting.
- The command line parameters are parsed using the `getopt()` function. There is the -f flag, which is used to specify the filename of the Particles that are used as input, the -t flag, which is optional, but can be used to specify the end time of the simulation (the simulation always starts at time 0) and the -e flag, which is also optional and can be used to specify the step size of the simulation (of course step size and end time need to have the same time unit)

### Task 3
- Using the Parameters start_time = 0, end_time = 1500 and delta_t=0.014 as well as the provided eingabe-sonne.txt as input to the programm, we get files that yield the following simulation in Paraview (using a glyph filter and colour to visualize the force):

https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/0a9f6b0d-07b4-4f3f-ae74-80bfd243aea1

<br>

- Looking at the masses of the celestial bodys in the input file yields:
  <br><br>
   **1. body:** xyz coordinates are 0.0 0.0 0.0 and the mass is 1.0 . Assuming the masses are normalized to the mass of the sun this would mean that this celestial body is the sun and has a mass of  $`1.9855×10^{30} kg`$ [^1]. It is by far the heaviest of the four and therefore stays at the center and is almost not influenced by the forces the others exert on him. It is the innermost planet in the simulation that does not move.
    



  **2. body:** xyz coordinates are 0.0 1.0 0.0 and the mass is 3.0e-6. If all the masses are normalized and 1 is the mass of the sun, then this planet is most likely the earth as $` 3.0×10^{-6} \space (sun \space unit) = 3.0×10^{-6} \cdot 1.9855×10^{30} kg \approx 5.972×10^{24} kg`$ and  $`5.972×10^{24} kg`$ corresponds to the mass of the earth [^1]. It is the planet closest to the sun (in the beginning), which can be seen by the xyz coordinates and as the simulation shows it has the highest orbital period/ circles the sun the fastest on a stable orbit.
  
  Orbit(approximately):
![orbit1_1](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/3aa2ced7-46ca-4f04-960c-8f0cc13798b3)


  **3. body:** xyz coordinates are 0.0 5.36 0.0 and the mass is 9.55e-4. Again if one removes the normalization, where the mass of the sun is 1 then $`9.55×10^{-4} \space (sun \space unit) = 9.55×10^{-4} \cdot 1.9855×10^{30} kg \approx 1.8987×10^{27} kg`$ this yields the mass of jupiter [^1]. It is the second closest planet (in the beginning) to the sun and just like the earth has a stable orbit around the sun. Jupiter has the second highest orbital period in our simulation.
  
   Orbit(approximately):
  ![orbit2_2](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/54e6e334-fc39-48f8-b96b-5aeea4e66ada)



  **4. body:** xyz coordinates are 34.75 0.0 0.0 and the mass is 1.0e-14. Again denormalization yields $`1.0×10^{-14} \space (sun \space unit) = 1.0×10^{-14}  \cdot 1.9855×10^{30} kg \approx 1.9855×10^{16} kg`$. Here the classification is a bit harder as there are no planets or moons with this mass. The fourth  body is far lighter than the others and it seems the only kind of celestial bodys with a size this small are asteroids. Therefore it might be a asteroid, but the classification is not as clear as the previous ones.

   Orbit(approximately):
  ![orbit3_3](https://github.com/Grazvy/PSEMolDyn_GroupB/assets/101070208/eb17c16e-a6f4-420d-8413-d92e0b09fd61)



    
### Task 4







[^1]: https://en.wikipedia.org/wiki/List_of_gravitationally_rounded_objects_of_the_Solar_System


**Simulation:**












