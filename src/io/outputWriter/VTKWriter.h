/*
 * VTKWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "models/Particle.h"
#include "vtk-unstructured.h"
#include "io/outputWriter/Writer.h"

#include <list>

namespace outputWriter {

/**
 * This class implements the functionality to generate vtk output from
 * particles.
 */
class VTKWriter : public Writer {

private:
    /**
   * set up internal data structures and prepare to plot a particle.
   */
    void initializeOutput(int numParticles);

    /**
     * plot type, mass, position, velocity and force of a particle.
     *
     * @note: initializeOutput() must have been called before.
     */
    void plotParticle(Particle &p);

    /**
     * writes the final output file.
     *
     * @param filename the base name of the file to be written.
     * @param iteration the number of the current iteration,
     *        which is used to generate an unique filename
     */
    void writeFile(const std::string &filename, int iteration);

    VTKFile_t *vtkFile;

public:
  VTKWriter();

  virtual ~VTKWriter();

  void plotParticles(ParticleContainer &particles, const std::string &filename, int iteration) override;

};

} // namespace outputWriter
