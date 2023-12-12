#include <list>
#include <string>

class Particle;


class Checkpointer{
    public:
        static void writeCheckpoint(std::list<Particle>& particles,const std::string filename);
        static void readCheckpoint(std::list<Particle>& particles,const std::string filename);

};