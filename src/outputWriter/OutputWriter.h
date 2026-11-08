#pragma once
#include <filesystem>
#include <memory>

#include "ParticleContainer.h"

namespace output_writer {

/**
 * @brief Formats for outputs the simulation can export
 */
enum class OutputFormat {
    kXyz,  ///< Plain text XYZ file, only contains Positions
#ifdef ENABLE_VTK_OUTPUT
    kVtk,       ///< VTK unstructured file (.vtu)
    kVtkAscii,  ///< VTK unstructured file (.vtu) as ascii rep
#endif
};

/**
 * @brief Interface for writing Particles
 */
class OutputWriter {
   public:
    virtual ~OutputWriter() = default;

    /**
     * @brief Writes the Particles to the file
     * @param particles particles to write
     * @param iteration Iteration to write, used in the file name
     */
    virtual void Write(const ParticleContainer &particles, int iteration) = 0;

    /**
     * @brief Factory for a Writer in the specified @p format
     *
     * The files get written into subdirs in dir not dir itself
     *
     * @param format File format
     * @param dir path for the output folder
     * @param prefix File name prefix, files get named: `<prefix>_<iteration>.<extension>`
     * @return Writer in the @p format
     */
    static std::unique_ptr<OutputWriter> MakeWriter(OutputFormat format,
                                                    const std::filesystem::path &dir,
                                                    std::string prefix);

    /**
     * @brief Uses @p writers to write the particles to files
     * @param particles Particles to write
     * @param writers Writers to write the particles
     * @param iteration Iteration, used for file name
     */
    static void PlotParticles(const ParticleContainer &particles,
                              const std::vector<std::unique_ptr<OutputWriter>> &writers,
                              int iteration);
};

}  // namespace output_writer
