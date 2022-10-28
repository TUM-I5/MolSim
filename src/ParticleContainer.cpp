
#include "Particle.h"
#include "FileReader.h"
#include <vector>

class ParticleContainer{
    private:
        std::vector<Particle> particles;
    public:
        ParticleContainer(){
        }

        ParticleContainer(char* filename){
            FileReader fileReader;
            fileReader.readFile(particles, filename);
        }

        std::vector<Particle> getParticles(){
            return particles;
        }

        Particle& getParticle(int i){
            if(0<=i && i<particles.size()){
                return particles[i];
            }
            else{
                throw std::runtime_error("Tried to access Particle with index out of bounds!\n");
            }
        }

        void forAllParticles(void (function)(Particle& p)){
            std::for_each(particles.begin(), particles.end(), [&](Particle &g) {function(g);});
        }

        void forAllPairs(void (function)(Particle& p1, Particle& p2)){
            for (u_int32_t i = 0; i < particles.size(); i++) {
                for (u_int32_t j = i + 1; j < particles.size(); j++) {
                    Particle &p1 = particles[i];
                    Particle &p2 = particles[j];
                    function(p1, p2);
                }
            }
        }

};