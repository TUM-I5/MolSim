MolSim
===
# Group E #
Members:
* Layla
* Marcus
* Sophy

# Code #
* Link:     https://github.com/sophykk/PSEMolDyn-GroupE.git
* Branch:   master
* Compiler: g++ 11.4.0
* Commit ID: f9f433e1c33c99b7893a2302ee24df2df8109b2e
* Build and execute instructions:
 ```
 mkdir build
 cd build
 ccmake ..
 make
 ./MolSim ..input/eingabe-sonne.txt <1000> <0.014> (the first argument represents the end_time and the second argument represents the delta_t)
 
 Doxygen documentation: 
 make doc_doxygen
 
 then for the visualisation go into ParaView and open the beforehand generated MD_vtk folder/group, hit apply and use the glyph filter with the according configurations on it
```
# Report #
## Task 2 ##

- force, velocity, and position calculation with additional norm calculation

## Task 3 ##

- the darkest particle represents Halley's Comet
- the one that changes colour from orange to red and does not move is the sun
- the blue particle orbiting the sun is Earth
- and the remaining particle, orbiting at a larger radius from the sun is Jupiter

- Attention: Halley's comet only appears from the second half onwards because of the zoom
- Animation 32x: https://github.com/sophykk/PSEMolDyn-GroupE/assets/149163154/629f1b51-1df8-4d08-b975-17488b4b1a5d

## Task 4 ##

- Which software design patterns are suitable for encapsulation?
  - Adapter, Bridge, Strategy pattern

-  What might be an easy and extensible way to use different methods for I/O and for calculating the force between particles?
   - using the Strategy pattern for encapsulating formulas and I/O into a new class

- What can you do with a system like Doxygen, what is it useful for?
  - Automated code documentation and code structure visualisation
