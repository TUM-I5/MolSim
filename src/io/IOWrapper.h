//
// Created by alex on 14.11.2022.
//

#pragma once

#include <type_traits>
#include <list>
#include "data/Particle.h"
#include "io/InputLoader.h"
#include "io/outputWriter/VTKWriter.h"
#include "FileReader.h"
#include "BodyReader.h"

namespace io {
    /**
     * Interface to all IO functionality.
     * By setting @param LOADER to either FileReader or BodyReader, the other template parameters can be omitted.
     * */
    template<typename LOADER, typename LOCATOR_p=const char*, void (*LOAD_p)(LOCATOR_p, std::list<Particle>&, double&, double&) = nullptr>
    class IOWrapper {
    private:
        /**
         * Defining function signature
         * */
        using LOAD_FUNCTION = void (*)(LOCATOR_p, std::list<Particle>&, double&, double&);

        /**
         * Compile time if else for expressions
         * */
        template<bool B, LOAD_FUNCTION T, LOAD_FUNCTION F>
        struct cond_nt { constexpr static const LOAD_FUNCTION type = T; };

        /**
         * Compile time if else for expressions
         * */
        template<LOAD_FUNCTION T, LOAD_FUNCTION F>
        struct cond_nt<false, T, F> { constexpr static const LOAD_FUNCTION type = F; };

        /**
         * Compile time if else for types, same as std::condition actually
         * */
        template<bool B, typename T, typename F>
        struct cond_t { using type = T; };

        /**
         * Compile time if else for types, same as std::condition actually
         * */
        template<typename T, typename F>
        struct cond_t<false, T, F> { using type = F; };

        /**
         * Defines the actual type of the LOCATOR
         * */
        using LOCATOR = typename cond_t<std::is_same<LOADER, FileReader>::value, const char*,
                typename cond_t<std::is_same<LOADER, BodyReader>::value, const char*, LOCATOR_p>::type
        >::type;

        /**
         * Defines the function pointer to the loader function at compile time
         * */
        constexpr static const LOAD_FUNCTION LOAD = cond_nt<std::is_same<LOADER, FileReader>::value, &FileReader::readFile,
                cond_nt<std::is_same<LOADER, BodyReader>::value, &BodyReader::readFile, LOAD_p>::type
        >::type;

        /**
         * Internal input loader instance.
         * */
        InputLoader<LOCATOR, LOAD> inputLoader;

        /**
         * Internal VTKWriter instance
         * */
        outputWriter::VTKWriter vtkWriter;

    public:
        /**
         * Creates an IOWrapper
         * @param loc is the location of the input data.
         * */
        explicit IOWrapper(LOCATOR loc) : inputLoader(loc) {
        }

        /**
         * Delegates to InputLoader::reload
         * */
        inline void reload() {
            inputLoader.reload();
        }

        /**
         * Delegates to InputLoader::getParticles
         * */
        inline void getParticles(std::vector<Particle> &buf) {
            inputLoader.getParticles(buf);
        }

        /**
         * Delegates to InputLoader::getEpsilon
         * */
        inline double getEpsilon() {
            return inputLoader.getEpsilon();
        }

        /**
         * Delegates to InputLoader::getSigma
         * */
        inline double getSigma() {
            return inputLoader.getSigma();
        }

        /**
         * Writes all particles in VTK format
         * */
        void writeParticlesVTK(ParticleContainer& pc, const std::string& outputFolder, const std::string& outputBaseName, int iteration) {
            vtkWriter.initializeOutput(pc.size());
            pc.forAllParticles([&](Particle& p){vtkWriter.plotParticle(p);});
            vtkWriter.writeFile(outputFolder + outputBaseName, iteration);
        }
    };

    extern std::shared_ptr<io::IOWrapper<io::BodyReader>> ioWrapper;
} // io
