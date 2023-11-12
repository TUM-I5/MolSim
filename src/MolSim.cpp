
#include "inputHandling/FileReader.h"
#include "particleModel/ParticleContainer.h"
#include "inputHandling/CuboidGenerator.h"
#include "Simulation.h"




#include <iostream>
#include <string>
#include <unistd.h>

int main(int argc, char *argsv[])
{
    //initialize default values
    double end_time = 5;
    double delta_t = 0.0002;
    //
    char *filename;
    ParticleContainer particleContainer;
    FileReader fileReader;

    auto msg = "Usage ./MolSim [-e<double>] [-t<double>] -f<String>\n"
               " -e<double>:      gives the end_time of the simulation\n"
               " -t<double>:      gives the step size used for the simulation\n"
               " -f<String>:      gives the filename from which the initial state\n"
               "                  of the Particles is read, these are the particles\n"
               "                  that will get simulated\n"
               "\n"
               "Returns:\n"
               "                  several .vtu files that can be used for visualisation in Paraview\n";

    std::cout << "Hello from MolSim for PSE!" << std::endl;
    int opt;
    while ((opt = getopt(argc, argsv, "t:e:f:h")) != -1)
    {
        switch (opt)
        {
            case 't':
                try
                {
                    delta_t = std::stod(optarg);
                    std::cout << "delta_t: " << delta_t << std::endl;
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << "Invalid argument for delta_t" << e.what() << std::endl;
                }
                catch (const std::out_of_range &e)
                {
                    std::cerr << "The delta_t is Out of range" << e.what() << std::endl;
                }
                break;
            case 'e':
                try
                {
                    end_time = std::stod(optarg);
                    std::cout << "end_time: " << end_time << std::endl;
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << "Invalid argument for the endtime" << e.what() << std::endl;
                }
                catch (const std::out_of_range &e)
                {
                    std::cerr << "The endtime is Out of Range" << e.what() << std::endl;
                }

                break;
            case 'f':
                filename = optarg;
                break;
            case 'h':
                std::cout << msg;
                return 0;
            default:
                std::cerr << msg;
                return 1;
        }
    }




    auto cuboids = fileReader.readCuboidFile(filename);

    addCuboids(particleContainer,cuboids);
    

    Simulation::runSimulation(particleContainer, end_time, delta_t);

    return 0;
}
