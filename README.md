# MolSim GroupF

## Members
  - Sasha Marola
  - Campbell Krulewski
  - Jakob Schulz

## Build without VTK
```bash
cmake -S . -B build
cmake --build build -j
```
## Options 

| Option                 | Effect                             |
|------------------------|------------------------------------|
| -DENABLE_VTK_OUTPUT=ON | generate VTK files in `output/VTK` |
| -DENABLE_DOXYGEN=OFF   | dont create the doc_doxygen target |

run instead with additional option `-DENABLE_VTK_OUTPUT=ON` to generate VTK files in `output/VTK` if running with `vtk` format
```bash
cmake -S . -B build -DENABLE_VTK_OUTPUT=ON
cmake --build build -j
```

## Documentation

Generate the Doxygen documentation into `build/html` (open `build/html/index.html`)
```bash
cmake -S . -B build
cmake --build build --target doc_doxygen
```

## Run

```bash
./build/MolSim <input file> [--dt <delta time>] [--start <start time>] [--end <end time>] [--format <formats>]
```

| Option           | Default  | Effect                                                                                 |
|------------------|----------|----------------------------------------------------------------------------------------|
| `input file`     | -        | Particle input data                                                                    |
| `--dt`           | 0.014    | Time step size per iteration                                                           |
| `--end`          | 1000     | End time                                                                               |
| `--start`        | 0        | Start time                                                                             |
| `--format`       | `xyz`    | Formats to write the output as. comma separated. `xyz`, `vtk` (binary), `vtka` (ASCII) |
| `--write_every`  | 10       | Changes how often the output is written                                                |
| `-o`, `--output` | `output` | Sets the output Directory                                                              |

Formats `vtk` and `vtka` are only available when built with `-DENABLE_VTK_OUTPUT=ON`

`./build/MolSim --help` to list options

### Examples:
```bash
./build/MolSim input/eingabe-sonne.txt
./build/MolSim input/eingabe-sonne.txt --dt 0.014 --end 1000 --format vtk,xyz
```

## Input file

The simulation input file is JSON. It contains a cuboids array; each cuboid defines its origin, particle counts, grid spacing, particle mass, and initial velocity. Multiple cuboids can be specified, each with different values.
Example input:
```json
{
  "version": 1,
  "cuboids": [
    {
      "origin": [0.0, 0.0, 0.0],
      "particle_counts": [10, 8, 6],
      "spacing": 0.1,
      "mass": 1.0,
      "initial_velocity": [0.0, 0.0, 0.0]
    },
    {
      "origin": [2.0, 0.0, 0.0],
      "particle_counts": [4, 4, 4],
      "spacing": 0.2,
      "mass": 2.0,
      "initial_velocity": [0.0, 1.0, 0.0]
    }
  ]
}

```

## Results
results are contained in the folder `output`
