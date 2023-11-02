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