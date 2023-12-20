#include <list>
#include <string>

class Particle;
class CellContainer;

/**
 * @namespace Checkpointer
 * @brief Contains functions related to checkpointing particles from a CellContainer.
 */
namespace Checkpointer {

    /**
     * @brief Creates a checkpoint by writing serialized particles into a specified file.
     *        For every particle, the corresponding sigma and epsilon are written into the
     *        file as well.
     *          
     *
     * @param particles Reference to a list of Particle objects to be serialized and written into file.
     * @param filename The name of the file where particle checkpoint is saved.
     */
    void writeCheckpoint(std::list<Particle>& particles, const std::string filename);

    /**
     * @brief Creates a checkpoint by storing the information of particles from a container into a file.
     *
     * @param container Reference to the CellContainer containing particles to be stored.
     * @param filename The name of the file to store the particles checkpoint in.
     */
    void storeCheckpointparticles(CellContainer& container, std::string filename);

    /**
     * @brief Reads the data of several particles and their corresponding sigma and epsilon from a file into
     *        a list of (particle,sigma,epsilon) tuples. The file has to be a file, 
     *        that was created by the `writeCheckpoint` / `storeCheckpointparticles` methods, 
     *        in order to have the correct file format.
     *
     * @param particles Reference to a list of tuples containing Particle, sigma, and epsilon.
     * @param filename The name of the file from which the particles checkpoint is read.
     */
    void readCheckpoint(std::list<std::tuple<Particle, double, double>>& particles, const std::string filename);

    /**
     * @brief Reads checkpointed particles from a file and adds them to a given CellContainer.
     *        Again the file has to be a file that was created from the 
     *        `writeCheckpoint` / `storeCheckpointparticles` methods, in order 
     *        to have the correct file format.
     *
     * @param container Reference to the CellContainer to which the checkpointed particles are added.
     * @param filename The name of the file from which the particles checkpoint is read.
     */
    void addCheckpointparticles(CellContainer& container, const std::string filename);


};