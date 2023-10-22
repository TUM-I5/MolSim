# MolSim

[![Build Project](https://github.com/ManuelLerchner/MolSim-WS23-24/actions/workflows/build-project.yml/badge.svg)](https://github.com/ManuelLerchner/MolSim-WS23-24/actions/workflows/build-project.yml)
[![Build Docs](https://github.com/ManuelLerchner/MolSim-WS23-24/actions/workflows/build-docs.yml/badge.svg)](https://github.com/ManuelLerchner/MolSim-WS23-24/actions/workflows/build-docs.yml)

Code for the practical course PSE: Molecular Dynamics by group C (WS 2023/24).

**Group members:**

- Manuel Lerchner
- Tobias Eppacher
- Daniel Safyan

## Build

### Without Doxygen

1. Create and enter into the build directory: `mkdir build && cd build`
2. Run cmake: `cmake ..` to configure the project.
3. Run `make` to build the program.

### With Doxygen

1. Create and enter into the build directory: `mkdir build && cd build`
2. Run cmake: `cmake .. -D BUILD_DOC_DOXYGEN=ON` to the cmake command.
3. Run `make` to build the program. Run `make docs` to build the documentation.

## Run

1. Run `./MolSim FILENAME` to run the program. FILENAME is the path to the input file.
