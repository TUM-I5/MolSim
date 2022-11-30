//
// Created by jan on 11/29/22.
//

#include "io/input/sim_input/XMLReader.h"


namespace io::input {
    XMLReader::XMLReader() = default;

    XMLReader::~XMLReader() = default;

    void XMLReader::readFile(const char *filename, std::list<Particle> &particles,
                             std::unordered_map<io::input::names, std::string> &arg_map) {
        std::unique_ptr<simulation_t> simulation;
        xml_schema::properties properties;
        properties.no_namespace_schema_location("file://XMLFormat.xsd"); //?

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
                arg_map.emplace(timeStepSize, std::to_string(simulation->TimeStepSize().get()));
            }
            else {
                arg_map.emplace(timeStepSize, std::to_string(default_delta_t));
            }

            if (simulation->ForceCalculation().Gravity().present()) {
                arg_map.emplace(forceCalculation, "gravity"); //Couldn't use .get() for some reason
            }
            else if (simulation->ForceCalculation().LennardJones().present()) {
                if (simulation->ForceCalculation().LennardJones()->Epsilon().present()) {
                    arg_map.emplace(epsilon, std::to_string(simulation->ForceCalculation().LennardJones()->Epsilon().get()));
                }
                if (simulation->ForceCalculation().LennardJones()->Sigma().present()) {
                    arg_map.emplace(sigma, std::to_string(simulation->ForceCalculation().LennardJones()->Sigma().get()));
                }
            }
            else {
                output::loggers::general->debug("This really shouldn't happen. No ForceCalculation was specified despite it being mandatory. Using default...");
            }

            if (simulation->AverageBrownianMotion().present()) {
                arg_map.emplace(brown, std::to_string(simulation->AverageBrownianMotion().get()));
            }
            else {
                arg_map.emplace(brown, std::to_string(default_brown));
            }

            if (simulation->SimulationStrategy().Naive().present()) {
                arg_map.emplace(linkedCell, "0");
            }
            else if (simulation->SimulationStrategy().LinkedCell().present()) {
                arg_map.emplace(linkedCell, "1");

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

                arg_map.emplace(boundingBox_X0, std::to_string(default_box_size));
                arg_map.emplace(boundingBox_X1, std::to_string(default_box_size));
                arg_map.emplace(boundingBox_X2, std::to_string(default_box_size));

                arg_map.emplace(boundCondFront, default_boundary_cond_str);
                arg_map.emplace(boundCondRear, default_boundary_cond_str);
                arg_map.emplace(boundCondLeft, default_boundary_cond_str);
                arg_map.emplace(boundCondRight, default_boundary_cond_str);
                arg_map.emplace(boundCondTop, default_boundary_cond_str);
                arg_map.emplace(boundCondBottom, default_boundary_cond_str);
            }

            if (simulation->Dimensions().present()) {
                arg_map.emplace(dimensions, std::to_string(simulation->Dimensions().get()));
            }
            else {
                arg_map.emplace(dimensions, std::to_string(default_dims));
            }

            // <!-- Misc -->

            if (simulation->LogLevel().present()) {
                arg_map.emplace(logLevel)
            }



        }
    }
}