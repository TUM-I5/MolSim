//
// Created by alex on 14.11.2022.
//

#pragma once

#include <type_traits>
#include <list>
#include <unordered_map>

#include "data/Particle.h"
#include "io/input/sim_input/InputLoader.h"
#include "io/output/VTKWriter.h"
#include "io/input/sim_input/FileReader.h"
#include "io/input/sim_input/BodyReader.h"
#include "defaults.h"
#include "io/input/arg_names.h"
#include "io/input/sim_input/XMLReader.h"

namespace io {
    /**
     * Interface to all IO functionality.
     * By setting @param LOADER to either FileReader or BodyReader, the other template parameters can be omitted.
     * */
    template<typename LOADER, typename LOCATOR_p=const char *, void (*LOAD_p)(LOCATOR_p, std::list<Particle> &,
                                                                              std::unordered_map<io::input::names, std::string> &) = nullptr>
    class IOWrapper {
    private:
        /**
         * Defining function signature
         * */
        using LOAD_FUNCTION_p = void (*)(LOCATOR_p, std::list<Particle> &,
                                         std::unordered_map<io::input::names, std::string> &);

        /**
         * Compile time if else for expressions
         * */
        template<bool B, LOAD_FUNCTION_p T, LOAD_FUNCTION_p F>
        struct cond_nt {
            constexpr static const LOAD_FUNCTION_p type = T;
        };

        /**
         * Compile time if else for expressions
         * */
        template<LOAD_FUNCTION_p T, LOAD_FUNCTION_p F>
        struct cond_nt<false, T, F> {
            constexpr static const LOAD_FUNCTION_p type = F;
        };

        /**
         * Compile time if else for types, same as std::condition actually
         * */
        template<bool B, typename T, typename F>
        struct cond_t {
            using type = T;
        };

        /**
         * Compile time if else for types, same as std::condition actually
         * */
        template<typename T, typename F>
        struct cond_t<false, T, F> {
            using type = F;
        };

        /**
         * Defines the actual type of the LOCATOR
         * */
        using LOCATOR = typename cond_t<std::is_same<LOADER, input::FileReader>::value, const char *,
                typename cond_t<std::is_same<LOADER, input::BodyReader>::value, const char *,
                typename cond_t<std::is_same<LOADER, input::XMLReader>::value, const char *, LOCATOR_p>::type>::type>::type;

        /**
         * Defines the function pointer to the loader function at compile time
         * */
        constexpr static const LOAD_FUNCTION_p LOAD = cond_nt<std::is_same<LOADER, input::FileReader>::value, &input::FileReader::readFile,
                cond_nt<std::is_same<LOADER, input::BodyReader>::value, &input::BodyReader::readFile,
                cond_nt<std::is_same<LOADER, input::XMLReader>::value, &input::XMLReader::readFile, LOAD_p>::type>::type>::type;

        /**
         * Internal input loader instance.
         * */
        input::InputLoader <LOCATOR, LOAD> inputLoader;

        /**
         * Internal VTKWriter instance
         * */
        outputWriter::VTKWriter vtkWriter;

        /**
         * Argument map. Stores argument by key. Default key names specified in io::input::names
         * */
        std::unordered_map<io::input::names, std::string> arg_map;

    public:
        /**
         * Creates an IOWrapper
         * @param loc is the location of the input data.
         * */
        explicit IOWrapper(LOCATOR loc) : inputLoader(loc, arg_map) {
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
         * Returns a read only view to the argument map
         * */
        const std::unordered_map<io::input::names, std::string>& getArgMap () const {
            return arg_map;
        }

        /**
         * Writes all particles in VTK format
         * */
        void
        writeParticlesVTK(ParticleContainer &pc, const std::string &outputFolder, const std::string &outputBaseName,
                          int iteration) {
            vtkWriter.initializeOutput(pc.activeSize());
            pc.forAllParticles([&](Particle &p) { vtkWriter.plotParticle(p); });
            vtkWriter.writeFile(outputFolder + outputBaseName, iteration);
        }
    };
} // io
