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


## Running the program

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