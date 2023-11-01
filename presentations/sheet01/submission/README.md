# Sheet 1: Team C

Members of **Team C**:

* Manuel Lerchner
* Tobias Eppacher
* Daniel Safyan

## Code

* Link:     <https://github.com/ManuelLerchner/MolSim-WS23-24>
* Branch:   master
* Revision: <TODO>
* Compiler: gcc 13.1.0 (But should work most up to date compilers)

## Report

### Task 1: "First steps"

* There is not much to say about this task. We downloaded `doxygen` and `paraview` and cloned the repository. Everything worked fine, as we were all able to seemlessly install the required software and to compile the code.

### Task 2: "Completion of the program frame"

1. **Graviational force calculation**
   * Implementing the force calculations between the planets was quite easy, because we could just copy the formula from the slides. However, we still managed to mess up the formula and ended up with repelling planets instead. This was a funny looking bug, but it was fortunately easy to fix. Afterwards the simulation looked as expected.
2. **Creating VTK Output Files**
   * This fix was also quite easy, as we just had to change a single function to use another `OutputWriter` class, which was already implemented. This worked without any problems.

### Task 3: "Simulation of Halleys comet"

* Simulating the system worked flawelessly and the movement of the planets looked as expected. A video of the simulation, can be found in the `submisssion` folder. The video is called `halleys_comet.mp4`.
* We also played around with `paraview` and tried to create some nice looking visualizations. We ended up coloring each body according to its look in the real world. The sun is yellow, the earth is green, jupiter is orange and halleys comet is brown. To put a scale reference into the video, we also added circles with the average orbit radius for the planets. The earth orbits per definition at 1 AU (in average). Jupiter at 5.2 AU.
* We found it quite cool, that we could actually see the slight elliptical orbits of all the planets which is caused by real world physics (And maybe a bit by the numerical integration method ^^).
* Halleys comet comes close to the sun and then flies out again. Just as expected, as it follows a highly elliptical orbit.

### Task 4: "Refactoring and documentation"

* We refactored the code to use `std::vector` instead of `std::list` to store the particles and encapsulated it in its own class `ParticleContainer`, where we collect the references of the objects in our system. The reason for using `std::vector` is, that the data is saved linearly which increases chache locality, thus accelerating the simulation. `std::array` would do the same, but as the number of particles is not known at compile time (which is required for `std::array`) it can't be used. 
* We decided to also create an interface for force calculations between to particles (`ForceCalculator`). This way we can create multiple implementations of it, and switch the forces to be used for a simulation seamlessly without having to change the code in the simulation loop.
* Similar to the force calculation we abstracted the integration method, as simulations with different integration methods can produce quite different results. For the time being we only have one integration method, but this will probably change in the future and we wanted to be prepared for this.
* To also keep input and output as modular as possible, we encapsulated it into a wrapper class, therefore, when we want to read input files or write our calculated results into files, only the implementation inside the wrapper has to be changed and the rest of the code is unaffected. (Note: This could also allows for fancy things like automatic detection of the input file format)
* Lastly, to keep the main file as clean as possible, we encapsulated the simulation loop and the setup of the simulation into its own class `Simulation`. This way the main file can be focused on CLI, feeding the simulation class with the required information (based on the CLI arguments) and only triggering the simulation process.
* As stated in the README building with doxygen is enabled via `cmake -D BUILD_DOC_DOXYGEN=ON ..` and then `make doc_doxygen`. The documentation can then be found in `build` folder. If `graphviz` is installed, the documentation will also contain some nice UML diagrams.

### Misc

* We already played around a bit with the github actions and created some basic workflows to build the code and automatically deploy the documentation. This worked quite well. The actions can be found in the [repository](https://github.com/ManuelLerchner/MolSim-WS23-24/tree/master/.github/workflows). The doxygen documentation is automatically built and deployed to github pages hosted at [https://manuellerchner.github.io/MolSim-WS23-24/](https://manuellerchner.github.io/MolSim-WS23-24/).
* We also looked into the github project management features and created a [project board](https://github.com/users/ManuelLerchner/projects/4) to keep track of the tasks. This board is automated and automatically updates itself when we create new issues or pull requests, begin work on features or finish issues. We also created a [milestone](https://github.com/ManuelLerchner/MolSim-WS23-24/milestone/1) for the first sheet and assigned all issues to this milestone. This way we can kept track of the progress of the sheet and could easily see how much work is left to do.
* We tried simulating other initial conditions, and were able to successfully simulate a "periodic figure 8" orbit. We used the initial conditions from [here](http://three-body.ipb.ac.rs/sV_sol.php?id=0)
* As the users of our program might want to check the progress of a running simulation we also added a progress bar, displayed in the terminal.
