#include "Checkpointer.h"
#include "particleModel/storage/Particle.h"
#include <fstream>
#include <iostream>





void Checkpointer::writeCheckpoint(std::list<Particle>& particles,const std::string filename){
    std::ofstream out_file(filename, std::ios::binary);
    if (out_file.is_open()) {
        // Serialize list
        for(Particle& particle : particles){
            out_file << particle;
        }
        out_file.close();
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }
}


void Checkpointer::readCheckpoint(std::list<Particle>& particles,const std::string filename){
    std::ifstream in_file(filename);
    if(in_file.is_open()){
        Particle particle;
        while (in_file >> particle)
        {
            particles.push_back(particle);
        }
        in_file.close();
    }else{
        std::cerr << "Unable to open file for reading" << std::endl;
    }
}