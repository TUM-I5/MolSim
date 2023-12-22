# Group B
## Project Information

**Members:**
Yuchen Zhao


**Build/Usage:**
```
mkdir build && cd build
ccmake ..

#for executable
make

#for usage type -h or no arguments, yields:
Usage ./MolSim [-l<String>] [-p] [-o] -f<String>
 Info:              See the /input folder for the parameters.xsd schema, in which 
                    program arguments should be specified
 -f<String>:        gives the filename of an .xml file, that has to follow
                    the xsd schema defined in input/parameters.xsd.
                    from this file all programm arguments / options will be read(see README)
 -l<String>:        specifies the level of logging, e.g. how fine grained programm logs are.
                    can either be "off" "trace", "debug", "info", "error" or "critical".
                    The default level is "debug".
 -h                 prints a help message
 -p                 if the flag is set, the programm will measure the time for the execution.
                    therefore no vtk output and no logging will happen (specifing a log level at
                    the same time is undefined behaviour)

#for documentation
make doc_doxygen 
```

**Notes:**
In week 4 we did this assignment separately. For Task 1-3, please refer to [Project Part 123](https://github.com/Grazvy/PSEMolDyn_GroupB).
For Task 4-5, please refer to [Project Part 45](https://github.com/NanaYCZ/MolSim/). The main work is shown in branch "yuchen", while the attempt of performance improvement is shown in branch "performence_enhancement_pow612".
For more details, please refer to README.md in submission/week4.

**Notes:**
Call ./MolSim with no arguments or the -h argument to get a help message about the 
command line arguments and what is being returned by the executable. This file should probably be viewed on GitHub, as some media embeddings might not work in e.g. an IDE 





