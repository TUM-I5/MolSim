//
// Created by alex on 14.11.2022.
//

#pragma once

#include <type_traits>
#include <list>
#include <unordered_map>
#include <fstream>

#include "data/Particle.h"
#include "io/input/sim_input/InputLoader.h"
#include "io/output/VTKWriter.h"
#include "io/input/sim_input/FileReader.h"
#include "io/input/sim_input/BodyReader.h"
#include "defaults.h"
#include "io/input/arg_names.h"
#include "io/input/sim_input/XMLReader.h"
#include "io/input/sim_input/types.h"
#include "io/input/Configuration.h"

namespace io {
    /**
     * Interface to all IO functionality.
     * */
    class IOWrapper {
    private:
        /**
         * Internal input loader instance.
         * */
        input::InputLoader<const char *, io::input::XMLReader::readFile> xmlLoader;

        /**
         * Internal input loader instance.
         * */
        input::InputLoader<const char *, io::input::FileReader::readFile> fileLoader;

        /**
         * Internal input loader instance.
         * */
        input::InputLoader<const char *, io::input::BodyReader::readFile> bodyLoader;

        /**
         * Internal VTKWriter instance
         * */
        outputWriter::VTKWriter vtkWriter;

        /**
         * Argument map. Stores argument by key. Default key names specified in io::input::names
         * */
        std::unordered_map<io::input::names, std::string> arg_map;

        /**
         * Currently Active load type
         * */
        input::type loadType;

    public:
        /**
         * Creates an IOWrapper
         * @param loc is the location of the input data.
         * */
        explicit IOWrapper(input::type lt, const std::string &loc = "") :
                xmlLoader(loc.c_str(), arg_map),
                fileLoader(loc.c_str(), arg_map),
                bodyLoader(loc.c_str(), arg_map),
                loadType(lt) {
        }

        /**
         * @brief Sets the loader type to lt
         * */
        [[maybe_unused]] void setLoadType(input::type lt) {
            loadType = lt;
        }


        /**
         * Delegates to InputLoader::reload
         * */
        inline void reload() {
            switch (loadType) {
                case io::input::type::XML: {
                    xmlLoader.reload();
                    return;
                }
                case io::input::type::FILE: {
                    fileLoader.reload();
                    return;
                }
                case io::input::type::BODY: {
                    bodyLoader.reload();
                    return;
                }
                default:
                    return;
            }
        }

        /**
         * Updates the locator with l by delegating to the input loader.
         * */
        void setLocator(const std::string &l) {
            xmlLoader.setLocator(l.c_str());
            fileLoader.setLocator(l.c_str());
            bodyLoader.setLocator(l.c_str());
        }

        /**
         * Delegates to InputLoader::getParticles
         * */
        inline void getParticles(std::vector<Particle> &buf) {
            switch (loadType) {
                case io::input::type::XML: {
                    xmlLoader.getParticles(buf);
                    return;
                }
                case io::input::type::FILE: {
                    fileLoader.getParticles(buf);
                    return;
                }
                case io::input::type::BODY: {
                    bodyLoader.getParticles(buf);
                    return;
                }
                default:
                    return;
            }
        }

        /**
         * Returns a read only view to the argument map
         * */
        const std::unordered_map<io::input::names, std::string> &getArgMap() const {
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

        /**
         * Write Checkpoint file using XMLFormat.xsd generated files.
         * */
        void writeCheckpoint(ParticleContainer &pc, io::input::Configuration &config, int iteration, double currentTime) {
            std::ofstream os;
            os.open(config.get<io::input::names::outputFilePath>() + config.get<io::input::names::outputFileName>() +
                    ".cp");

            //create mandatory objects for simulation_t
            forceCalculation_t ForceCalculation;
            switch (config.get<io::input::forceCalculation>()) {
                case sim::physics::force::gravity:
                    ForceCalculation.Gravity(gravity_t());
                    break;
                case sim::physics::force::lennardJones: {
                    lennardJones_t lj;
                    lj.Epsilon(config.get<io::input::epsilon>());
                    lj.Sigma(config.get<io::input::epsilon>());
                    ForceCalculation.LennardJones(lj);
                    break;
                }
                case sim::physics::force::lennardJonesOMP: {
                    lennardJonesOMP_t lj;
                    lj.Epsilon(config.get<io::input::epsilon>());
                    lj.Sigma(config.get<io::input::epsilon>());
                    ForceCalculation.LennardJonesOMP(lj);
                    break;
                }
                case sim::physics::force::lennardJonesCell: {
                    lennardJonesCell_t lj;
                    lj.Epsilon(config.get<io::input::epsilon>());
                    lj.Sigma(config.get<io::input::epsilon>());
                    ForceCalculation.LennardJonesCell(lj);
                    break;
                }
                case sim::physics::force::lennardJonesGravity: {
                    lennardJonesGravity_t lj;
                    lj.Epsilon(config.get<io::input::epsilon>());
                    lj.Sigma(config.get<io::input::epsilon>());
                    ForceCalculation.LennardJonesGravity(lj);
                    break;
                }
                default:
                    break;
            }

            simulationStrategy_t SimulationStrategy;
            if (config.get<io::input::linkedCell>()) {
                std::unordered_map<sim::physics::bounds::type, std::string> bMap = {{sim::physics::bounds::outflow,    "Outflow"},
                                                                                    {sim::physics::bounds::reflecting, "Reflecting"},
                                                                                    {sim::physics::bounds::periodic,   "Periodic"}};
                SimulationStrategy.LinkedCell(
                        linkedCell_t(
                                boundaries_t(
                                        posDVector_t(
                                                config.get<io::input::boundingBox_X0>(),
                                                config.get<io::input::boundingBox_X1>(),
                                                config.get<io::input::boundingBox_X2>()
                                        ),
                                        boundaryBehavior_t(bMap[config.get<io::input::boundCondFront>()]),
                                        boundaryBehavior_t(bMap[config.get<io::input::boundCondRear>()]),
                                        boundaryBehavior_t(bMap[config.get<io::input::boundCondLeft>()]),
                                        boundaryBehavior_t(bMap[config.get<io::input::boundCondRight>()]),
                                        boundaryBehavior_t(bMap[config.get<io::input::boundCondTop>()]),
                                        boundaryBehavior_t(bMap[config.get<io::input::boundCondBottom>()])
                                ),
                                config.get<io::input::rCutoff>())
                );
            } else {
                SimulationStrategy.Naive(naive_t());
            }

            file_t FileType;
            cp_particle_list_t CPParticles;
            cp_particle_list_t::CPParticle_sequence sequence;
            pc.forAllParticles([&](Particle &p) {
                sequence.push_back(cp_particle_t(
                        dvector_t(p.getX()[0], p.getX()[1], p.getX()[2]),
                        dvector_t(p.getV()[0], p.getV()[1], p.getV()[2]),
                        dvector_t(p.getF()[0], p.getF()[1], p.getF()[2]),
                        dvector_t(p.getOldF()[0], p.getOldF()[1], p.getOldF()[2]),
                        p.getM(),
                        p.getEpsilon(),
                        p.getSigma(),
                        p.getType()
                ));
            });
            CPParticles.CPParticle(sequence);
            FileType.Checkpoint(checkpoint_t(iteration, CPParticles));

            simulation_t Simulation(ForceCalculation, SimulationStrategy, FileType);

            //create optional objects for simulation_t
            output_t OutputFile;
            OutputFile.FolderPath(config.get<io::input::outputFilePath>());
            OutputFile.OutputFileName(config.get<io::input::outputFileName>());
            Simulation.OutputFile(OutputFile);
            Simulation.StartTime(currentTime);
            Simulation.EndTime(config.get<io::input::endTime>());
            Simulation.TimeStepSize(config.get<io::input::delta_t>());

            //TODO continue with PositionCalculation

            os.flush();
            os.close();
        }
    };
} // io
