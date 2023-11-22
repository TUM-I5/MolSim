#pragma once

#include "particles/ParticleContainer.h"
#include "simulation_schema-pskel.hxx"

/**
 * @brief Class to read particle data from an XML file
 */
class configuration_pimpl : public configuration_pskel {
   private:
    std::string fps_;

   public:
    using configuration_pskel::fps;
    virtual void fps(const std::string& f) { fps_ = f; }

    void readFile(const std::string& filepath, ParticleContainer& particle_container);

    std::string get_fps() { return fps_; }
};