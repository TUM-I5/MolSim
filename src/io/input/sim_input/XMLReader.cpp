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
        //xml_schema::properties properties;
        //properties.no_namespace_schema_location("XMLFormat.xsd"); //?

        try {

            // <!-- IO -->

            if(simulation->OutputFile().present()) {
                if (simulation->OutputFile()->FolderPath().present()) {
                    arg_map.emplace(outputFilePath, simulation->OutputFile()->FolderPath().get());
                }
                else {
                    arg_map.emplace(outputFilePath, default_output_folder);
                }
                if (simulation->OutputFile()->OutputFileName().present()) {
                    arg_map.emplace(outputFileName, simulation->OutputFile()->OutputFileName().get());
                }
                else {
                    arg_map.emplace(outputFileName, default_output_base_name);
                }
            }

            // <!-- Calculation Decisions -->

            if (simulation->StartTime().present()) {
                arg_map.emplace(startTime, std::to_string(simulation->StartTime().get()));
            }
            else {
                arg_map.emplace(startTime, std::to_string(default_start_time));
            }

            if (simulation->EndTime().present()) {
                arg_map.emplace(endTime, std::to_string(simulation->EndTime().get()));
            }
            else {
                arg_map.emplace(endTime, std::to_string(default_end_time));
            }

            if (simulation->TimeStepSize().present()) {
                arg_map.emplace(delta_t, std::to_string(simulation->TimeStepSize().get()));
            }
            else {
                arg_map.emplace(delta_t, std::to_string(default_delta_t));
            }

            if (simulation->ForceCalculation().Gravity().present()) {
                arg_map.emplace(forceCalculation, "gravity"); //Couldn't use .get() for some reason
            }
            else if (simulation->ForceCalculation().LennardJones().present()) {
                arg_map.emplace(forceCalculation, "lennardJones");

                if (simulation->ForceCalculation().LennardJones()->Epsilon().present()) {
                    arg_map.emplace(epsilon, std::to_string(simulation->ForceCalculation().LennardJones()->Epsilon().get()));
                }
                if (simulation->ForceCalculation().LennardJones()->Sigma().present()) {
                    arg_map.emplace(sigma, std::to_string(simulation->ForceCalculation().LennardJones()->Sigma().get()));
                }
            } else if (simulation->ForceCalculation().LennardJonesCell().present()) {
                arg_map.emplace(forceCalculation, "lennardjonescell");

                if (simulation->ForceCalculation().LennardJonesCell()->Epsilon().present()) {
                    arg_map.emplace(epsilon, std::to_string(simulation->ForceCalculation().LennardJonesCell()->Epsilon().get()));
                }
                if (simulation->ForceCalculation().LennardJonesCell()->Sigma().present()) {
                    arg_map.emplace(sigma, std::to_string(simulation->ForceCalculation().LennardJonesCell()->Sigma().get()));
                }
            } else if (simulation->ForceCalculation().LennardJonesOMP().present()) {
                arg_map.emplace(forceCalculation, "lennardjonesOMP");

                if (simulation->ForceCalculation().LennardJonesOMP()->Epsilon().present()) {
                    arg_map.emplace(epsilon, std::to_string(simulation->ForceCalculation().LennardJonesOMP()->Epsilon().get()));
                }
                if (simulation->ForceCalculation().LennardJonesOMP()->Sigma().present()) {
                    arg_map.emplace(sigma, std::to_string(simulation->ForceCalculation().LennardJonesOMP()->Sigma().get()));
                }
            }
            else {
                output::loggers::general->debug("This really shouldn't happen. No ForceCalculation was specified despite it being mandatory. Using default...");
                arg_map.emplace(forceCalculation, default_force_type);
            }

            if (simulation->PositionCalculation().present()) {
                arg_map.emplace(positionCalculation, simulation->PositionCalculation().get());
            }
            else {
                arg_map.emplace(positionCalculation, default_pos_type);
            }

            if (simulation->VelocityCalculation().present()) {
                arg_map.emplace(velocityCalculation, simulation->VelocityCalculation().get());
            }
            else {
                arg_map.emplace(velocityCalculation, default_vel_type);
            }

            double brown_val;
            if (simulation->AverageBrownianMotion().present()) {
                arg_map.emplace(brown, std::to_string(simulation->AverageBrownianMotion().get()));
                brown_val = simulation->AverageBrownianMotion().get();
            }
            else {
                arg_map.emplace(brown, std::to_string(default_brown));
                brown_val = default_brown;
            }

            if (simulation->SimulationStrategy().Naive().present()) {
                arg_map.emplace(linkedCell, "0");
            }
            else if (simulation->SimulationStrategy().LinkedCell().present()) {
                arg_map.emplace(linkedCell, "1");

                arg_map.emplace(rCutoff, std::to_string(simulation->SimulationStrategy().LinkedCell()->CutoffRadius()));

                arg_map.emplace(boundingBox_X0, std::to_string(simulation->SimulationStrategy().LinkedCell()->BoundaryBox().BoxSize().X()));
                arg_map.emplace(boundingBox_X1, std::to_string(simulation->SimulationStrategy().LinkedCell()->BoundaryBox().BoxSize().Y()));
                arg_map.emplace(boundingBox_X2, std::to_string(simulation->SimulationStrategy().LinkedCell()->BoundaryBox().BoxSize().Z()));

                arg_map.emplace(boundCondFront, simulation->SimulationStrategy().LinkedCell()->BoundaryBox().Front());
                arg_map.emplace(boundCondRear, simulation->SimulationStrategy().LinkedCell()->BoundaryBox().Rear());
                arg_map.emplace(boundCondLeft, simulation->SimulationStrategy().LinkedCell()->BoundaryBox().Left());
                arg_map.emplace(boundCondRight, simulation->SimulationStrategy().LinkedCell()->BoundaryBox().Right());
                arg_map.emplace(boundCondTop, simulation->SimulationStrategy().LinkedCell()->BoundaryBox().Top());
                arg_map.emplace(boundCondBottom, simulation->SimulationStrategy().LinkedCell()->BoundaryBox().Bottom());
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

            int dims_val;
            if (simulation->Dimensions().present()) {
                arg_map.emplace(dimensions, std::to_string(simulation->Dimensions().get()));
                dims_val = simulation->Dimensions().get();
            }
            else {
                arg_map.emplace(dimensions, std::to_string(default_dims));
                dims_val = default_dims;
            }

            // <!-- Misc -->

            if (simulation->LogLevel().present()) {
                arg_map.emplace(logLevel, std::to_string(simulation->LogLevel().get()));
            }
            else {
                arg_map.emplace(logLevel, std::to_string(default_log_level));
            }

            if (simulation->Benchmark().present()) {
                arg_map.emplace(benchmark, "true");
                if (simulation->Benchmark()->BenchmarkType().FileBenchmark().present()) {
                    arg_map.emplace(benchmarkType, "fileBenchmark");
                }
                else {
                    arg_map.emplace(benchmarkType, "defaultBenchmark");
                    if (simulation->Benchmark()->BenchmarkType().DefaultBenchmark()->MaximumBodySize().present()) {
                        arg_map.emplace(benchMaxBodySize, std::to_string(simulation->Benchmark()->BenchmarkType().DefaultBenchmark()->MaximumBodySize().get()));
                    }
                    else {
                        arg_map.emplace(benchMaxBodySize, std::to_string(default_bench_maxBody));
                    }
                }

                if (simulation->Benchmark()->IterationCount().present()) {
                    arg_map.emplace(benchIterationCount, std::to_string(simulation->Benchmark()->IterationCount().get()));
                }
                else {
                    arg_map.emplace(benchIterationCount, std::to_string(default_bench_iterations));
                }
            }

            for (shapeList_t::Shape_const_iterator s (simulation->ShapeList().Shape().begin()); s != simulation->ShapeList().Shape().end(); ++s) {
                Body body;

                if (s->Particle().present()) {
                    body.shape = Shape::particle;

                    dvectorToEigenVector3d(s->Particle()->Position(), body.fixpoint);
                    dvectorToEigenVector3d(s->Particle()->Velocity(), body.start_velocity);

                    if (s->Particle()->Mass() == 0) {
                        output::loggers::general->warn("Particle has a mass of 0, which is illegal. Skipping this particle...");
                        continue;
                    }

                    body.mass = s->Particle()->Mass();
                    ParticleGenerator::generateParticle(body.fixpoint, body.start_velocity, body.mass, particles);
                }
                else if (s->Cuboid().present()) {
                    body.shape = Shape::cuboid;

                    dvectorToEigenVector3d(s->Cuboid()->Position(), body.fixpoint);
                    dvectorToEigenVector3d(s->Cuboid()->Velocity(), body.start_velocity);

                    ivectorToEigenVector3d(s->Cuboid()->Dimensions(), body.dimensions);

                    if (s->Cuboid()->Mass() == 0 || s->Cuboid()->Spacing() == 0) {
                        output::loggers::general->warn("Cuboid has a mass or spacing of 0, which is illegal. Skipping this cuboid...");
                        continue;
                    }
                    body.distance = s->Cuboid()->Spacing();
                    body.mass = s->Cuboid()->Mass();

                    ParticleGenerator::generateCuboid(body, brown_val, particles, dims_val);
                }

                else if (s->Sphere().present()) {
                    body.shape = Shape::sphere;

                    dvectorToEigenVector3d(s->Sphere()->Position(), body.fixpoint);
                    dvectorToEigenVector3d(s->Sphere()->Velocity(), body.start_velocity);

                    body.dimensions << s->Sphere()->Radius(), s->Sphere()->Radius(), s->Sphere()->Radius();

                    if (s->Sphere()->Mass() == 0 || s->Sphere()->Spacing() == 0) {
                        output::loggers::general->warn("Sphere has a mass or spacing of 0, which is illegal. Skipping this sphere...");
                        continue;
                    }
                    body.distance = s->Sphere()->Spacing();
                    body.mass = s->Sphere()->Mass();
                    ParticleGenerator::generateSphere(body, brown_val, particles, dims_val);
                }

                else {
                    output::loggers::general->debug("An unknown shape was detected. This really shouldn't happen. Skipping...");
                    continue;
                }

                // handle body and create particles

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