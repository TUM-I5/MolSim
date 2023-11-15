/**
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"

#include <iostream>

#include "io/logger/Logger.h"
#include "utils/ArrayUtils.h"

Particle::Particle(int type_arg) {
    type = type_arg;
    f = {0., 0., 0.};
    old_f = {0., 0., 0.};
    Logger::logger->debug("Particle created with");
}

Particle::Particle(const Particle& other) {
    x = other.x;
    v = other.v;
    f = other.f;
    old_f = other.old_f;
    m = other.m;
    type = other.type;
    Logger::logger->debug("Particle created");
}

Particle::Particle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg, int type_arg) {
    x = x_arg;
    v = v_arg;
    m = m_arg;
    type = type_arg;
    f = {0., 0., 0.};
    old_f = {0., 0., 0.};
    Logger::logger->debug("Particle created");
}

Particle::~Particle() {
    Logger::logger->debug("Particle destroyed");
}

void Particle::setX(const std::array<double, 3>& x_arg) { x = x_arg; }

void Particle::setV(const std::array<double, 3>& v_arg) { v = v_arg; }

void Particle::setF(const std::array<double, 3>& f_arg) { f = f_arg; }

void Particle::setOldF(const std::array<double, 3>& old_f_arg) { old_f = old_f_arg; }

const std::array<double, 3>& Particle::getX() const { return x; }

const std::array<double, 3>& Particle::getV() const { return v; }

const std::array<double, 3>& Particle::getF() const { return f; }

const std::array<double, 3>& Particle::getOldF() const { return old_f; }

double Particle::getM() const { return m; }

int Particle::getType() const { return type; }

std::string Particle::toString() const {
    std::stringstream stream;
    stream << "Particle: X:" << x << " v: " << v << " f: " << f << " old_f: " << old_f << " type: " << type;
    return stream.str();
}

bool Particle::operator==(Particle& other) {
    return (x == other.x) and (v == other.v) and (f == other.f) and (type == other.type) and (m == other.m) and (old_f == other.old_f);
}

std::ostream& operator<<(std::ostream& stream, Particle& p) {
    stream << p.toString();
    return stream;
}
