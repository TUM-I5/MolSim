#include <list>
#include <string>

class Particle;
class CellContainer;


class Checkpointer{
public:
    static void addCheckpointparticles(CellContainer& container,const std::string filename);
    static void writeCheckpoint(std::list<Particle>& particles,const std::string filename);

    static void storeCheckpointparticles(CellContainer& container,std::string filename);
    static void readCheckpoint(std::list<std::tuple<Particle,double,double>>& particles,const std::string filename);
};