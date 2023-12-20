#include "Checkpointer.h"
#include "particleModel/storage/Particle.h"
#include "particleModel/updating/CellCalculator.h"
#include "particleModel/storage/CellContainer.h"
#include <fstream>
#include <iostream>



namespace Checkpointer{

void writeCheckpoint(std::list<Particle>& particles,const std::string filename){
    std::ofstream out_file(filename, std::ios::binary);
    if (out_file.is_open()) {
        // Serialize list
        for(Particle& particle : particles){
            out_file << particle << ' ' 
            << sigma_mixed[particle.getType()][particle.getType()] << ' '
            << epsilon_mixed[particle.getType()][particle.getType()] << ' ';
        }
        out_file.close();
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }
}


void readCheckpoint(std::list<std::tuple<Particle,double,double>>& particles,const std::string filename){
    std::ifstream in_file(filename);
    if(in_file.is_open()){
        try{
        Particle particle;
        while (in_file >> particle)
        {   
            double sigma, epsilon ;
            if(! (in_file >> sigma))
                break;

            if(! (in_file >> epsilon))
                break;
            
            particles.push_back({particle,sigma,epsilon});
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

void storeCheckpointparticles(CellContainer& container,const std::string filename){
    std::list<Particle> result_particles = container.to_list();
    Checkpointer::writeCheckpoint(result_particles,filename);
}


void addCheckpointparticles(CellContainer& container,std::string filename){
    std::list<std::tuple<Particle,double,double>> predefined_particles;
    Checkpointer::readCheckpoint(predefined_particles,filename);
    for(auto& particle_sigma_epsilon : predefined_particles){
        container.addParticle(std::get<0>(particle_sigma_epsilon),
                                      std::get<1>(particle_sigma_epsilon),
                                      std::get<2>(particle_sigma_epsilon));
    }

}

}