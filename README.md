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

| Option                                  | Effect                                                                                                     |
|-----------------------------------------|------------------------------------------------------------------------------------------------------------|
| -DENABLE_VTK_OUTPUT=ON                  | generate VTK files in `output/VTK`                                                                         |
| -DENABLE_DOXYGEN=OFF                    | dont create the doc_doxygen target                                                                         |
| <a id="LOG_LEVEL"></a> -DLOG_LEVEL=INFO | Set the compile time log level. Options are: `TRACE`, `DEBUG`, `INFO`, `WARN`, `ERROR`, `CRITICAL`, `OFF`. |

run instead with additional option `-DENABLE_VTK_OUTPUT=ON` to generate VTK files in `output/VTK` if running with `vtk` format
```bash
cmake -S . -B build -DENABLE_VTK_OUTPUT=ON
cmake --build build -j
```

### Benchmark build

Disable compile time logging for Benchmarking:
```bash
cmake -S . -B build-bench -DCMAKE_BUILD_TYPE=Release -DLOG_LEVEL=OFF
cmake --build build-bench -j
./build-bench/MolSim input/eingabe-2bodies-lennard-jones.json --dt 0.0002 --end 5 --bench
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

| Option                   | Default  | Effect                                                                                                                                |
|--------------------------|----------|---------------------------------------------------------------------------------------------------------------------------------------|
| `input file`             | -        | Particle input data                                                                                                                   |
| `--dt`                   | 0.014    | Time step size per iteration                                                                                                          |
| `--end`                  | 1000     | End time                                                                                                                              |
| `--start`                | 0        | Start time                                                                                                                            |
| `--format`               | `xyz`    | Formats to write the output as. comma separated. `xyz`, `vtk` (binary), `vtka` (ASCII)                                                |
| `--write_every`          | 10       | Changes how often the output is written                                                                                               |
| `-o`, `--output`         | `output` | Sets the output Directory                                                                                                             |
| `--benchmark`, `--bench` |          | Times the raw simulation time                                                                                                         |
| `--log-level`            | `info`   | Set the log level. Options: `trace`, `debug`, `info`, `warn`, `error`, `off`. Cant go below the Compile time [LOG_LEVEL](#LOG_LEVEL). |

Formats `vtk` and `vtka` are only available when built with `-DENABLE_VTK_OUTPUT=ON`

`./build/MolSim --help` to list options

### Examples:
```bash
./build/MolSim input/eingabe-sonne.txt
./build/MolSim input/eingabe-sonne.txt --dt 0.014 --end 1000 --format vtk,xyz
./build/MolSim input/eingabe-2bodies-lennard-jones.json --dt 0.0002 --end 5
```

## Input file

### JSON Input

Contains a cuboids array; each cuboid defines its origin, particle counts, grid spacing, particle mass, and initial velocity. Multiple cuboids can be specified, each with different values.

Example in `input/eingabe-2bodies-lennard-jones.json`

### TXT Input

A list of positions, velocities, and masses

Example in `input/eingabe-sonne.txt`

## Results
results are contained in the folder `output`
