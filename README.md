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

## Results
results are contained in the folder `output`
