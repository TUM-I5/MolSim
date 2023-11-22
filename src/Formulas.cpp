//
// Created by Layla Zadina on 09.11.2023.
//

#include "Formulas.h"
#include "utils/ArrayUtils.h"
#include "Particle.h"
#include "outputWriter/VTKWriter.h"
#include <cmath>
#include <array>
#include <utility> // std::pair, std::make_pair




/**
   * L2 norm (xi,xj)
   */

//template <double, std::size_t N>
double Formulas::secondNorm(const std::array<double, 3> &arr1) {
    double norm = 0.0;
    for (size_t i = 0; i < 3; i++) {
        norm += arr1[i] * arr1[i];
    }
    return std::sqrt(norm);
}

/**
   * Lennard-Jones potential
   */

double Formulas::calculatePotential(std::array<double, 3> &xi, std::array<double, 3> &xj, double sigma, double eps) {
    //U(xi, xj) = 4*eps((sigma/L2_norm(xi,xj))^12 - (sigma/L2_norm(xi,xj))^6)
    auto potential = 4 * eps * (pow((sigma / Formulas::secondNorm(xi - xj)), 12.0) -
                                pow((sigma / Formulas::secondNorm(xi - xj)), 6.0));
    return potential;
}

/**
   * Lennard-Jones force
   */

void Formulas::calculateLJForce(std::pair<Particle, Particle> ppair, double sigma, double eps) {
    auto force = (-24 * eps / pow(Formulas::secondNorm(ppair.first.Particle::getX() - ppair.second.Particle::getX()), 2.0))
                 * (pow((sigma / Formulas::secondNorm(ppair.first.Particle::getX() - ppair.second.Particle::getX())), 6.0)
                 - 2 * pow((sigma / Formulas::secondNorm(ppair.first.Particle::getX() - ppair.second.getX())), 12.0))
                 * (ppair.first.Particle::getX() - ppair.second.Particle::getX());
    ppair.first.Particle::setF(force);
    ppair.second.Particle::setF((-1) * force);
}

//could do that with particle pairs instead for faster calculation
void Formulas::calculateF(ParticleContainer pc) {

    for (auto &p1: pc.getParticles()) {
        std::array<double, 3> F_i{0., 0., 0.};
        for (auto &p2: pc.getParticles()) {
            // formula: Fij = ((mi * mj) / ||xi −xj||^3) * (xj − xi)
            std::array<double, 3> F_ij{};
            if (&p1 != &p2) {
                auto mul = p1.getM() * p2.getM() * (p2.getX() - p1.getX());

                for (int i = 0; i < 3; ++i) {
                    F_ij[i] = mul[i] / pow(Formulas::secondNorm((p1.getX() - p2.getX())), 3.0);
                    F_i[i] += F_ij[i];
                }
            }
            p1.setF(F_i);
            p2.setF((-1) * F_i);
        }
    }
}


void Formulas::calculateX(double delta_t, ParticleContainer pc) {
    for (auto &p: pc.getParticles()) {
        // formula: xi(tn+1) = xi(tn) + ∆t · vi(tn) + (∆t)^2 * (Fi(tn)/2mi)
        // Calculate xi(tn+1)

        auto xi_tn1 = p.getX() + delta_t * p.getV() + (delta_t * delta_t) / (2.0 * p.getM()) * p.getF();

        p.setX(xi_tn1);  // Update the position
    }
}

void Formulas::calculateV(double delta_t, ParticleContainer pc) {
    for (auto &p: pc.getParticles()) {
        // formula: vi(tn+1) = vi(tn) + ∆t * ((Fi(tn) + Fi(tn+1))/ 2mi)
        // Calculate the velocity at time tn+1

        auto vi_tn1 = p.getV() + delta_t / (2 * p.getM()) * (p.getOldF() + p.getF());
        p.setV(vi_tn1);
    }
}

void Formulas::plotParticles(int iteration, ParticleContainer pc) {

    std::string out_name("MD_vtk");

    outputWriter::VTKWriter writer;
    writer.initializeOutput(pc.size());
    for (auto &p: pc.getParticles()) {
        writer.plotParticle(p);
    }
    writer.writeFile(out_name, iteration);
}
