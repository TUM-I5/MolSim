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
        try{
        Particle particle;
        while (in_file >> particle)
        {
            particles.push_back(particle);
        }
        in_file.close();
        }catch(const std::exception& e){
            std::cerr << "After opening, wasn't able to read the Checkpoint file you provided" << std::endl;
            std::cerr << e.what() << std::endl;
            in_file.close();
        }
    }else{
        std::cerr << "Unable to open Checkpoint file for reading" << std::endl;
    }
}