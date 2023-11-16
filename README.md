# Molecular Dynamics Simulation

[![Build and Test](https://github.com/ManuelLerchner/MolSim-WS23-24/actions/workflows/build-project-test-project.yml/badge.svg)](https://github.com/ManuelLerchner/MolSim-WS23-24/actions/workflows/build-project-test-project.yml)
[![Build Docs](https://github.com/ManuelLerchner/MolSim-WS23-24/actions/workflows/build-docs.yml/badge.svg)](https://github.com/ManuelLerchner/MolSim-WS23-24/actions/workflows/build-docs.yml)

Code for the practical course PSE: Molecular Dynamics by group C (WS 2023/24).

**Group members:**

- Manuel Lerchner
- Tobias Eppacher
- Daniel Safyan

## Tools

### Build tools and versions

- Tested with `gcc 13.1.0`
- Tested with `CMake 3.28.0`
- Tested with `make 4.3`

### Dependencies

- Doxygen 1.10.0: (optional, only needed for documentation)
  - Graphviz: `sudo apt install graphviz` (optional, only needed for drawing UML diagrams in doxygen)
- Libxerces: `sudo apt install libxerces-c-dev`
- Boost Program Options: `sudo apt-get install libboost-program-options-dev`
- cmake-format: `sudo apt install cmake-format-13` (optional, only needed for formatting cmake files)

## Build

### Without Doxygen support

1. Create and enter into the build directory: `mkdir -p build && cd build`
2. Run cmake: `cmake ..` to configure the project.
3. Run `make -j` to build the program.

### With Doxygen support

1. Create and enter into the build directory: `mkdir -p build && cd build`
2. Run cmake: `cmake .. -D BUILD_DOC_DOXYGEN=ON` to configure the project.
3. Run `make -j` to build the program. Run `make doc_doxygen` to build the documentation.

## Run

### Run the program

- Enter the `build/project` directory after building the project.

- Run `./MolSim <FILENAME>` to run the program. `<FILENAME>` is the path to the input file. For more information on the possible input file formats see [Input File Formats](@ref InputFileFormats).

  - Excecute `./MolSim --help` to get a detailed list of all options, parameters and their default values.

  - An example run could look like this: `./MolSim ../../body_collision.cub -d 0.0002 -e 5`

### Run the tests

- Enter the `build/tests` directory after building the project.

- Run `ctest` or `./tests` to run the tests.

### Build the documentation

- Make sure the project is built **with** doxygen enabled.

- Enter the `build` directory after building the project.

- Run `make doc_doxygen` to build the documentation.

- The output can be found in `build/docs/html/index.html`.

- The documentation of the `master` branch can be found [here](https://manuellerchner.github.io/MolSim-WS23-24/).
