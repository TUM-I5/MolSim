# Group B
## Project Information

**Members:**
    Yuchen Zhao,
    Grazvydas Kuras,
    David Kratz

[Project Link](https://github.com/Grazvy/PSEMolDyn_GroupB)

**Last commit:** commit-id: df4c241   commit-msg: " Merge branch 'master' of https://github.com/Grazvy/PSEMolDyn_GroupB
"

**Build/Usage:**
```
mkdir build && cd build
ccmake ..

#for executable
make

#for usage type -h or no arguments, yields:
./MolSim [-e<double>] [-t<double>] [-l<String>] -f<String>
#-e<double>:        gives the end_time of the simulation
#-t<double>:        gives the step size used for the simulation
#-f<String>:        gives the filename from which the initial state of the Particles is read, these are the particles that will get simulated
#-l<String>:        specifies the level of logging, e.g. how fine grained programm logs are. String can either be "off", "trace", "debug", "info", "error" or "critical". The default level is "debug".

#for documentation
make doc_doxygen 
```

**Notes:**
Call ./MolSim with no arguments or the -h argument to get a help message about the 
command line arguments and what is being returned by the executable. This file should probably be viewed on GitHub, as some media embeddings might not work in e.g. an IDE 





