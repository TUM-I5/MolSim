//
// Created by jan on 11/29/22.
//

#include "io/input/sim_input/XMLReader.h"


namespace io::input {
    XMLReader::XMLReader() = default;

    XMLReader::~XMLReader() = default;

    void XMLReader::readFile(const char *filename, std::list<Particle> &particles,
                             std::unordered_map<io::input::names, std::string> &arg_map) {
        std::unique_ptr<simulation_t> simulation {Simulation(std::string{filename})};

        try {
            // we will prefer arguments from a checkpoint file to a normal one
            bool isCheckpoint = simulation->FileType().Checkpoint().present();
            /**Set str in arg map at name if present and if arg map has no value there or if this file is a checkpoint file*/
            auto setInMap = [&](names name, bool present, const std::string& def, const std::string& str){
                if (!present && !arg_map.contains(name)) {
                    arg_map.emplace(name, def);
                    return;
                }
                if (isCheckpoint || !arg_map.contains(name)) arg_map.emplace(name, str);
            };
            /**Set str in arg map at name if arg map has no value there or if this file is a checkpoint file*/
            auto setInMapND = [&](names name, const std::string& str) {
                if (isCheckpoint || !arg_map.contains(name)) arg_map.emplace(name, str);
            };
            std::string parseBuffer;
            auto setInMapVal = [&](names name, bool present, const std::string& def, const std::string& str){
                if (!present && !arg_map.contains(name)) {
                    arg_map.emplace(name, def);
                    parseBuffer = def;
                    return;
                }
                if (isCheckpoint || !arg_map.contains(name)) {
                    arg_map.emplace(name, str);
                    parseBuffer = str;
                }
                parseBuffer = arg_map.at(name);
            };


            // <!-- IO -->

            if(simulation->OutputFile().present()) {
                setInMap(outputFilePath, simulation->OutputFile()->FolderPath().present(), default_output_folder, simulation->OutputFile()->FolderPath().get());
                setInMap(outputFileName, simulation->OutputFile()->OutputFileName().present(), default_output_base_name, simulation->OutputFile()->OutputFileName().get());
            }

            // <!-- Calculation Decisions -->

            setInMap(startTime, simulation->StartTime().present(), std::to_string(default_start_time), std::to_string(simulation->StartTime().get()));
            setInMap(endTime, simulation->EndTime().present(), std::to_string(default_end_time), std::to_string(simulation->EndTime().get()));
            setInMap(delta_t, simulation->TimeStepSize().present(), std::to_string(default_delta_t), std::to_string(simulation->TimeStepSize().get()));

            if (simulation->ForceCalculation().Gravity().present()) {
                setInMapND(forceCalculation, "gravity");
            }
            else if (auto& lj = simulation->ForceCalculation().LennardJones(); lj.present()) {
                setInMapND(forceCalculation, "lennardJones");
                setInMap(epsilon, lj->Epsilon().present(), std::to_string(default_epsilon), std::to_string(lj->Epsilon().get()));
                setInMap(sigma, lj->Sigma().present(), std::to_string(default_sigma), std::to_string(lj->Sigma().get()));
            }
            else if (auto& lj = simulation->ForceCalculation().LennardJonesCell(); lj.present()) {
                setInMapND(forceCalculation, "lennardjonescell");
                setInMap(epsilon, lj->Epsilon().present(), std::to_string(default_epsilon), std::to_string(lj->Epsilon().get()));
                setInMap(sigma, lj->Sigma().present(), std::to_string(default_sigma), std::to_string(lj->Sigma().get()));
            }
            else if (auto& lj = simulation->ForceCalculation().LennardJonesOMP(); lj.present()) {
                setInMapND(forceCalculation, "lennardjonesOMP");
                setInMap(epsilon, lj->Epsilon().present(), std::to_string(default_epsilon), std::to_string(lj->Epsilon().get()));
                setInMap(sigma, lj->Sigma().present(), std::to_string(default_sigma), std::to_string(lj->Sigma().get()));
            }
            else if (auto& lj = simulation->ForceCalculation().LennardJonesGravity(); lj.present()) {
                setInMapND(forceCalculation, "lennardjonesgravity");
                setInMap(epsilon, lj->Epsilon().present(), std::to_string(default_epsilon), std::to_string(lj->Epsilon().get()));
                setInMap(sigma, lj->Sigma().present(), std::to_string(default_sigma), std::to_string(lj->Sigma().get()));
                setInMap(gGrav, lj->Sigma().present(), std::to_string(default_g_grav), std::to_string(lj->G_Grav().get()));
            }
            else {
                output::loggers::general->debug("This really shouldn't happen. No ForceCalculation was specified despite it being mandatory. Using default...");
                setInMapND(forceCalculation, default_force_type);
            }

            setInMap(positionCalculation, simulation->PositionCalculation().present(), default_pos_type, simulation->PositionCalculation().get());
            setInMap(velocityCalculation, simulation->VelocityCalculation().present(), default_vel_type, simulation->VelocityCalculation().get());

            setInMapVal(brown, simulation->AverageBrownianMotion().present(), std::to_string(default_brown), std::to_string(simulation->AverageBrownianMotion().get()));
            double brown_val = std::stod(parseBuffer);

            if (simulation->SimulationStrategy().Naive().present()) {
                setInMapND(linkedCell, "0");
            }
            else if (auto& lc = simulation->SimulationStrategy().LinkedCell(); lc.present()) {
                setInMapND(linkedCell, "1");
                setInMapND(rCutoff, std::to_string(lc->CutoffRadius()));
                setInMapND(boundingBox_X0, std::to_string(lc->BoundaryBox().BoxSize().X()));
                setInMapND(boundingBox_X1, std::to_string(lc->BoundaryBox().BoxSize().Y()));
                setInMapND(boundingBox_X2, std::to_string(lc->BoundaryBox().BoxSize().Z()));

                setInMapND(boundCondFront, lc->BoundaryBox().Front());
                setInMapND(boundCondRear, lc->BoundaryBox().Rear());
                setInMapND(boundCondLeft, lc->BoundaryBox().Left());
                setInMapND(boundCondRight, lc->BoundaryBox().Right());
                setInMapND(boundCondTop, lc->BoundaryBox().Top());
                setInMapND(boundCondBottom, lc->BoundaryBox().Bottom());
            }
            else {
                output::loggers::general->debug("This really shouldn't happen. No SimulationStrategy was specified despite it being mandatory. Using default...");

                arg_map.emplace(linkedCell, std::to_string(default_linked_cell));

                arg_map.emplace(boundingBox_X0, std::to_string(default_bound_x0));
                arg_map.emplace(boundingBox_X1, std::to_string(default_bound_x1));
                arg_map.emplace(boundingBox_X2, std::to_string(default_bound_x2));

                arg_map.emplace(boundCondFront, default_boundary_cond_str);
                arg_map.emplace(boundCondRear, default_boundary_cond_str);
                arg_map.emplace(boundCondLeft, default_boundary_cond_str);
                arg_map.emplace(boundCondRight, default_boundary_cond_str);
                arg_map.emplace(boundCondTop, default_boundary_cond_str);
                arg_map.emplace(boundCondBottom, default_boundary_cond_str);
            }


            setInMapVal(dimensions, simulation->Dimensions().present(), std::to_string(default_dims), std::to_string(simulation->Dimensions().get()));
            int dims_val = std::stoi(parseBuffer);

            if (auto& t = simulation->Thermostat(); t.present()) {
                setInMapND(thermoEnable, std::to_string(1));
                setInMapND(thermoTInit, std::to_string(t.get().T_Init()));
                setInMapND(thermoNTerm, std::to_string(t.get().N_Term()));
                setInMap(thermoTTarget, t.get().T_Target().present(), std::to_string(t.get().T_Init()), std::to_string(t.get().T_Target().get()));
                setInMap(thermoDelta_t, t.get().Delta_T().present(), std::to_string(default_delta_temp), std::to_string(t.get().Delta_T().get()));
            } else {
                setInMapND(thermoEnable, std::to_string(0));
            }


            // <!-- Misc -->

            setInMap(logLevel, simulation->LogLevel().present(), std::to_string(default_log_level), std::to_string(simulation->LogLevel().get()));
            setInMap(checkpointingEnable, simulation->EnableCheckpointing().present(), std::to_string(default_checkpointing),std::to_string(simulation->EnableCheckpointing().get()));

            if (simulation->Benchmark().present()) {
                setInMapND(benchmark, "1");
                if (simulation->Benchmark()->BenchmarkType().FileBenchmark().present()) {
                    setInMapND(benchmarkType, "file");
                }
                else {
                    setInMapND(benchmarkType, "default");
                    if (simulation->Benchmark()->BenchmarkType().DefaultBenchmark()->MaximumBodySize().present()) {
                        setInMapND(benchMaxBodySize, std::to_string(simulation->Benchmark()->BenchmarkType().DefaultBenchmark()->MaximumBodySize().get()));
                    }
                    else {
                        setInMapND(benchMaxBodySize, std::to_string(default_bench_maxBody));
                    }
                }
                setInMap(benchIterationCount, simulation->Benchmark()->IterationCount().present(), std::to_string(default_bench_iterations), std::to_string(simulation->Benchmark()->IterationCount().get()));
            }

            //handle body/particles according to file type
            if (simulation->FileType().Input().present()) {
                auto& shapeList = simulation->FileType().Input().get().ShapeList();
                for (const auto & s : shapeList.Shape()) {
                    Body body;

                    if (s.Particle().present()) {
                        body.shape = Shape::particle;

                        dvectorToEigenVector3d(s.Particle()->Position(), body.fixpoint);
                        dvectorToEigenVector3d(s.Particle()->Velocity(), body.start_velocity);

                        if (s.Particle()->Mass() == 0) {
                            output::loggers::general->warn("Particle has a mass of 0, which is illegal. Skipping this particle...");
                            continue;
                        }

                        body.mass = s.Particle()->Mass();
                        ParticleGenerator::generateParticle(body.fixpoint, body.start_velocity, body.mass, particles);
                    }
                    else if (s.Cuboid().present()) {
                        body.shape = Shape::cuboid;

                        dvectorToEigenVector3d(s.Cuboid()->Position(), body.fixpoint);
                        dvectorToEigenVector3d(s.Cuboid()->Velocity(), body.start_velocity);

                        ivectorToEigenVector3d(s.Cuboid()->Dimensions(), body.dimensions);

                        if (s.Cuboid()->Mass() == 0 || s.Cuboid()->Spacing() == 0) {
                            output::loggers::general->warn("Cuboid has a mass or spacing of 0, which is illegal. Skipping this cuboid...");
                            continue;
                        }
                        body.distance = s.Cuboid()->Spacing();
                        body.mass = s.Cuboid()->Mass();

                        ParticleGenerator::generateCuboid(body, brown_val, particles, dims_val);
                    }

                    else if (s.Sphere().present()) {
                        body.shape = Shape::sphere;

                        dvectorToEigenVector3d(s.Sphere()->Position(), body.fixpoint);
                        dvectorToEigenVector3d(s.Sphere()->Velocity(), body.start_velocity);

                        body.dimensions << s.Sphere()->Radius(), s.Sphere()->Radius(), s.Sphere()->Radius();

                        if (s.Sphere()->Mass() == 0 || s.Sphere()->Spacing() == 0) {
                            output::loggers::general->warn("Sphere has a mass or spacing of 0, which is illegal. Skipping this sphere...");
                            continue;
                        }
                        body.distance = s.Sphere()->Spacing();
                        body.mass = s.Sphere()->Mass();
                        ParticleGenerator::generateSphere(body, brown_val, particles, dims_val);
                    }

                    else {
                        output::loggers::general->debug("An unknown shape was detected. This really shouldn't happen. Skipping...");
                        continue;
                    }

                    // handle body and create particles

                }
            }
            else if (simulation->FileType().Checkpoint().present()) {
                setInMapND(simLastIteration, std::to_string(simulation->FileType().Checkpoint().get().LastIteration()));

                auto& cpParticles = simulation->FileType().Checkpoint().get().CPParticles();
                for (auto& cp : cpParticles.CPParticle()) {
                    Particle p {std::array<double, 3>{cp.Position().X(), cp.Position().Y(), cp.Position().Z()},
                                std::array<double, 3>{cp.Velocity().X(), cp.Velocity().Y(), cp.Velocity().Z()},
                                cp.Mass(), cp.Type()};
                    p.setF({cp.Force().X(), cp.Force().Y(), cp.Force().Z()});
                    p.setOldF({cp.OldForce().X(), cp.OldForce().Y(), cp.OldForce().Z()});
                    p.setSigma(cp.Sigma());
                    p.setEpsilon(cp.Epsilon());
                    particles.push_back(p);
                }
            }
            else {
                output::loggers::general->error("unsupported FileType in XML. Valid are Input or Checkpoint");
                exit(-1);
            }


        }
        catch (const xml_schema::exception &e) {
            output::loggers::general->error("The following exception occurred during the parsing of your XML input file:");
            output::loggers::general->error(e.what());
            exit(1);
        }
    }

    void XMLReader::dvectorToEigenVector3d(dvector_t const &dv, Eigen::Vector3d &ev) {
        ev << dv.X(), dv.Y(), dv.Z();
    }

    void XMLReader::ivectorToEigenVector3d(ivector_t const &dv, Eigen::Vector3d &ev) {
        ev << dv.X(), dv.Y(), dv.Z();
    }
}