/**
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include <fstream>
#include <list>

#include "containers/ParticleContainer.h"
#include "particles/Particle.h"

/**
 * @brief Class to write particle data to a xyz file
 *
 * This class implements the functionality to generate xyz output from
 * particles.
 */
class XYZWriter {
   public:
    XYZWriter();

    virtual ~XYZWriter();

    void plotParticles(const std::unique_ptr<ParticleContainer>& particles, const std::string& filename, int iteration) const;
};
