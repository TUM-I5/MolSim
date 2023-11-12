# PSE Molecular Dynamics WS23/24 - Group F

## Members
- Alp Kaan Aksu
- Berke Saylan
- Feryal Ezgi Aşkın

## Unit Testing 
We are using GoogleTest framework for the unit tests in this project, since our project is expected to be independent from the user system’s available libraries, we achieve self-containment by incorporating the necessary GoogleTest content directly into our project using the `FetchContent_Declare` function. The current latest GoogleTest version we are working with is v1.14.0, which we are downloading from this GitHub commit: https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip. For the unit tests we are creating a new test executable named `MolSimTest`, which contains all the files with unit tests, apart from the main executable `MolSim`. We then used `gtest_discover_tests` to automatically discover and configure tests for `MolSimTest`. The first unit tests we wrote are for the `ParticleContainer` class: 

`TestAddParticle:` Ensures that the size of the `ParticleContainer` is correctly updated after adding particles. 
Test: Verifies that the size of the container is 2 after adding particles during the setup. 

`TestRemoveParticle:` Validates that removing a particle from the container decrements its size. 
Test: Removes a particle from the container and checks if the size becomes 1. 

`TestApplyToAll:` Checks if applying a function to all particles in the container works as expected. 
Test: Utilizes a lambda function to set a boolean value in a map for each processed particle. Ensures that the function is applied to all particles in the container.

## Continous Integration 
In order to utilize continuous integration in the development cycle of our project, we use two GitHub Actions workflows that streamline both the development and documentation processes. The CMake workflow ensures consistent builds for different platforms (Windows, Linux) and compilers (GCC, Clang) by configuring and building the project with CMake, considering platform-specific requirements and compiler settings. With this workflow, we ensure that our Molecular Dynamics simulation has the basic software engineering qualities such as cross-platform compatibility, compiler independence and a standardised build process, regardless of the user's system settings. Simultaneously, we have also employed the GitHub Pages Deployment Workflow, which automates the deployment of Doxygen-generated documentation to GitHub Pages. This workflow generates Doxygen documentation, uploads it as an artifact and deploys it to GitHub Pages, providing accessible and up-to-date project documentation.

## Logging 
We again use the FetchContent module to acquire the `spdlog` library from its GitHub repository, ensuring our project's independence from user system libraries. The current latest `spdlog` library version we are working with is v1.12.0, which we are downloading from this GitHub commit:  https://github.com/gabime/spdlog.git. `spdlog` is linked to both the main executable (`MolSim`) and the test executable (`MolSimTest`), so that we can use efficient logging capabilities in tests as well. We opted for using spdlog functions instead of macros in our simulation. This decision is due to the greater flexibility offered by the features that allow dynamic customisation of logging details such as message format and logging level. Although we have not currently utilised these customisation options, we foresee their importance for future enhancements to the simulation. The ability to dynamically customise logging might be critical for more complex functions, and we consider the slight execution overhead that the features introduce to be acceptable, given the benefits they may provide for our future logging requirements.

## Improvements in Input File Processing 
We decided to switch from a `.txt` file to a `.json` file to specify simulation input parameters such as the video duration, the selected force calculation model, the width of the molecular cuboid, etc. First of all, using a `.json` file helps us to better structure our input file and parameters, which also makes parsing the input file much easier. Currently, there are many important input parameters in the simulation and it is likely that there will be even more complex parameters in the future. Therefore, the flexibility of JSON in representing different types of information and using different data structures helps with the hierarchical structure and readability, maintainability of the input file processing module. The `JSONReader::readFile` method uses the `nlohmann::json ` library to parse a given JSON file and return the parsed JSON object. In the `Simulation` constructor, we used this JSON object to initialize various simulation parameters. The values for parameters such as `endTime`, `deltaT`, `videoDuration`, `fps`, `out`, and `outputType` are directly extracted from the `simulation` section of the JSON file. Additionally, particle configurations specified under the `objects ` key are added to the simulation using the particle container. The choice of simulation model is determined based on the `model ` key, allowing for dynamic selection between different models such as the basic one or Lennard-Jones model with specific parameters like epsilon and sigma.

## Updates in Particle Simulation
### Particle Generator 
Instead of generating molecules one by one seperately, we make use of a Particle Generator class, which orchestrates the creation of a cubic region where particles are arranged in a 3D rectangular grid. Essential parameters such as position, size, mesh width, initial velocity, mass, and type ID define the characteristics of the cuboid. In this iteration of the project, we simulate the collision of two cuboids of different size.

### Brownian Motion 
Particles are in continuous motion driven by their thermal energy, a form of kinetic energy known as Brownian motion. This inherent randomness results in unpredictable fluctuations in particle direction and speed, however this randomness can be characterized and represented by specific probability distributions such as Maxwell Boltzmann distribution. In our simulation, we apply Brownian motion to the particles only once at the beginning of the simulation and not at each time step. In the `Simulation::run` method, we initialise the velocity of the particles with the sum of the entered velocity parameters for this cuboid and the velocity caused by Brownian motion, which is calculated using the Maxwell-Boltzmann distribution in the header with the same name.

### Lennard Jones Model
The Lennard Jones mathematical model is commonly used in molecular dynamics simulations to calculate the potential energies, hence forces between atoms or molecules. The model accounts for both attractive van der Waals forces at longer distances and repulsive forces at shorter distances, providing a realistic approximation of intermolecular interactions.
