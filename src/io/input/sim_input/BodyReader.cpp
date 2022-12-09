/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: Johnny, Alex
 */

#include "BodyReader.h"
#include "data/ParticleGenerator.h"
#include "data/Body.h"
#include "io/input/arg_names.h"
#include "io/output/Logging.h"
#include "defaults.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <Eigen>


static std::string toStringEigen(const Eigen::Vector3d &vect) {
    std::stringstream stream;
    stream << "[" << vect[0] << ", " << vect[1] << ", " << vect[2] << "]";
    return stream.str();
}

namespace io::input {
    BodyReader::BodyReader() = default;

    BodyReader::~BodyReader() = default;

    void BodyReader::readFile(const char *filename, std::list<Particle> &buffer,
                              std::unordered_map<io::input::names, std::string> &arg_map) {
        std::array<double, 3> x;
        std::array<double, 3> v;
        double m;
        int numBodyLines = 0;

        std::ifstream input_file(filename);
        std::string tmp_string;

        if (input_file.is_open()) {

            getline(input_file, tmp_string);
            io::output::loggers::general->debug("Read line: {}", tmp_string);

            while (tmp_string.empty() or tmp_string[0] == '#') {
                getline(input_file, tmp_string);
                io::output::loggers::general->debug("Read line: {}", tmp_string);
            }

            // get number of particles
            std::istringstream numstream(tmp_string);
            numstream >> numBodyLines;
            io::output::loggers::general->debug("Reading {}", numBodyLines);

            // handle all particles
            getline(input_file, tmp_string);
            io::output::loggers::general->debug("Read line: {}", tmp_string);

            //store bodies for now
            std::vector<Body> bodies;

            for (int i = 0; i < numBodyLines; i++) {
                std::istringstream datastream(tmp_string);

                //load position
                for (auto &xj: x)datastream >> xj;
                //load velocity
                for (auto &vj: v) datastream >> vj;

                if (datastream.eof()) {
                    io::output::loggers::general->error(
                            "Error reading file: eof reached unexpectedly reading from line: {}", i);
                    exit(-1);
                }
                datastream >> m;

                //Shape extensions starting here
                if (datastream.eof()) {

                    struct Body body{};
                    body.shape = Shape::particle;
                    body.fixpoint << x[0], x[1], x[2];
                    body.start_velocity << v[0], v[1], v[2];
                    body.mass = m;
                    bodies.emplace_back(body);
                } else {
                    struct Body body;
                    std::string shapeString;
                    datastream >> shapeString;
                    body.shape = shapeFromString(shapeString);
                    body.fixpoint << x[0], x[1], x[2];
                    body.start_velocity << v[0], v[1], v[2];
                    body.mass = m;
                    datastream >> body.dimensions[0];
                    datastream >> body.dimensions[1];
                    datastream >> body.dimensions[2];
                    datastream >> body.distance;
                    bodies.emplace_back(body);
                }

                getline(input_file, tmp_string);
                io::output::loggers::general->debug("Read line: {}", tmp_string);
            }

            //get epsilon, sigma, brown, dims or use default values
            //and skip comments above
            while (tmp_string.empty() or tmp_string[0] == '#') {
                if (input_file.eof()) break;
                getline(input_file, tmp_string);
                io::output::loggers::general->debug("Read line: {}", tmp_string);
            }
            std::istringstream datastream(tmp_string);
            std::string arg_buffer;
            // epsilon
            double eps;
            if (!datastream.eof()) {
                datastream >> arg_buffer;
                if (!arg_buffer.empty()) arg_map.emplace(io::input::names::epsilon, arg_buffer);
                eps = std::stod(arg_buffer);
                arg_buffer.clear();
            }
            else eps = default_epsilon;
            // sigma
            double sig;
            if (!datastream.eof()) {
                datastream >> arg_buffer;
                if (!arg_buffer.empty())arg_map.emplace(io::input::names::sigma, arg_buffer);
                sig = std::stod(arg_buffer);
                arg_buffer.clear();
            }
            else sig = default_sigma;
            // brown
            double brown_average;
            int dims;
            if (!datastream.eof()) {
                datastream >> arg_buffer;
                if (!arg_buffer.empty())arg_map.emplace(io::input::names::brown, arg_buffer);
                brown_average = std::stod(arg_buffer);
                arg_buffer.clear();
            } else brown_average = default_brown;
            //dims
            if (!datastream.eof()) {
                datastream >> arg_buffer;
                if (!arg_buffer.empty())arg_map.emplace(io::input::names::dimensions, arg_buffer);
                dims = std::stoi(arg_buffer);
                arg_buffer.clear();
            } else dims = default_dims;

            for (auto &body: bodies) {
                switch (body.shape) {
                    case cuboid:
                        io::output::loggers::general->debug(
                                "Cuboid with dimensions " + toStringEigen(body.dimensions) + " at fixpoint " +
                                toStringEigen(body.fixpoint) + "created");
                        ParticleGenerator::generateCuboid(body, brown_average, buffer, dims, sig, eps);
                        break;
                    case sphere:
                        if (body.dimensions[0] != body.dimensions[1] || body.dimensions[1] != body.dimensions[2]) {
                            body.dimensions[1] = body.dimensions[0];
                            body.dimensions[2] = body.dimensions[0];
                            io::output::loggers::general->info("Dimensions set to " + toStringEigen(body.dimensions) +
                                                               " in order to get a valid Sphere. Spheres with different dimensions in different directions are not allowed");
                        }
                        io::output::loggers::general->debug(
                                "Sphere with dimensions " + toStringEigen(body.dimensions) + " at fixpoint " +
                                toStringEigen(body.fixpoint) + "created");
                        ParticleGenerator::generateSphere(body, brown_average, buffer, dims, sig, eps);
                        break;
                    case particle:
                        io::output::loggers::general->debug(
                                std::string("Particle at coordinates [") + std::to_string(body.fixpoint[0]) +
                                std::string(", ") +
                                std::to_string(body.fixpoint[1]) + std::string(", ") +
                                std::to_string(body.fixpoint[2]) + std::string("] created"));
                        ParticleGenerator::generateParticle(body.fixpoint, body.start_velocity, body.mass, buffer, sig, eps);
                        break;
                    default:
                        io::output::loggers::general->error("Unknown body type specified!");
                        break;
                }
            }
        } else {
            io::output::loggers::general->error("Error: could not open file {}", filename);
            exit(-1);
        }
    }

    enum Shape BodyReader::shapeFromString(std::string &shape) {
        auto lowercase = [&](std::string &str) {
            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
        };
        auto compare = [&](const std::string &str0, const std::string &str1) {
            std::string a{str0};
            std::string b{str1};
            lowercase(a);
            lowercase(b);
            return a == b;
        };
        const std::array<std::string, num_shapes> strOfShape = {"Cuboid", "Sphere", "Particle"};
        for (size_t i = 0; i < num_shapes; i++) {
            if (compare(shape, strOfShape[i])) {
                return all_shapes[i];   //all shapes defined in Body.h
            }
        }
        io::output::loggers::general->error("Couldn't interpret {} as shape", shape);
        exit(-1);
    }
} // io::input