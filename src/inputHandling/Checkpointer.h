#include <list>
#include <string>

class Particle;


class Checkpointer{
    public:
        void writeCheckpoint(std::list<Particle>& particles,const std::string filename);
        void readCheckpoint(std::list<Particle>& particles,const std::string filename);

};