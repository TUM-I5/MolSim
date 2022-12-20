//
// Created by alex on 14.11.2022.
//

#pragma once

#include <type_traits>
#include <list>
#include <unordered_map>
#include <fstream>
#include <filesystem>

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
         * Initializes the output location.
         * */
        void initOutput(const std::string& of) {
            std::string tmp{of};
            if (!of.ends_with("/")) tmp = tmp.append("/");
            if (!std::filesystem::exists(tmp))
                std::filesystem::create_directory(tmp);
            else if (!std::filesystem::is_directory(tmp)) {
                io::output::loggers::general->error(tmp + ": is not a directory!");
                exit(-1);
            }
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
            std::string tmp{outputFolder};
            if (!outputFolder.ends_with("/")) tmp = tmp.append("/");
            vtkWriter.initializeOutput(pc.activeSize());
            pc.forAllParticles([&](Particle &p) { vtkWriter.plotParticle(p); });
            vtkWriter.writeFile(tmp + outputBaseName, iteration);
        }

        /**
         * Write Checkpoint file using XMLFormat.xsd generated files.
         * */
        void writeCheckpoint(ParticleContainer &pc, io::input::Configuration &config, int iteration, double currentTime) {
            std::string tmp{config.get<io::input::names::outputFilePath>()};
            if (!tmp.ends_with("/")) tmp = tmp.append("/");

            std::ofstream os;
            os.open(tmp + "cp_" + config.get<io::input::names::outputFileName>() + "_" + std::to_string(iteration) +
                    ".xml");

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

            simulation_t simulation(ForceCalculation, SimulationStrategy, FileType);

            //create optional objects for simulation_t
            output_t OutputFile;
            OutputFile.FolderPath(config.get<io::input::outputFilePath>());
            OutputFile.OutputFileName(config.get<io::input::outputFileName>());
            simulation.OutputFile(OutputFile);
            simulation.StartTime(currentTime);
            simulation.EndTime(config.get<io::input::endTime>());
            simulation.TimeStepSize(config.get<io::input::delta_t>());

            std::unordered_map<sim::physics::position::type, std::string> pMap = {{sim::physics::position::stoermerVelvet,    "StoermerVelvet"},
                                                                                {sim::physics::position::stoermerVelvetOMP,   "StoermerVelvetOMP"}};
            positionCalculation_t PositionCalculation(pMap[config.get<io::input::positionCalculation>()]);
            simulation.PositionCalculation(PositionCalculation);

            std::unordered_map<sim::physics::velocity::type, std::string> vMap = {{sim::physics::velocity::stoermerVelvet,    "StoermerVelvet"},
                                                                                  {sim::physics::velocity::stoermerVelvetOMP,   "StoermerVelvetOMP"}};
            velocityCalculation_t VelocityCalculation(vMap[config.get<io::input::velocityCalculation>()]);
            simulation.VelocityCalculation(VelocityCalculation);

            simulation.AverageBrownianMotion(config.get<io::input::brown>());
            simulation.Dimensions(dimension_t(config.get<io::input::dimensions>()));
            if(config.get<io::input::thermoEnable>()) {
                thermostat_t therm(config.get<io::input::thermoTInit>(), config.get<io::input::thermoNTerm>());
                therm.T_Target(config.get<io::input::thermoTTarget>());
                therm.Delta_T(config.get<io::input::thermoDelta_t>());
                simulation.Thermostat(therm);
            }

            simulation.LogLevel(logLevel_t(config.get<io::input::logLevel>()));
            //ignoring benchmark, since benchmark does not write checkpoints
            simulation.EnableCheckpointing(1);

            //Write output
            xml_schema::properties properties;
            properties.no_namespace_schema_location("XMLFormat.xsd");

            xml_schema::namespace_infomap infomap;
            const ::std::string& e = "UTF-8";
            xml_schema::flags flags = 0;

            Simulation(os, simulation, infomap, "UTF-8", flags);

            os.flush();
            os.close();
        }
    };
} // io
