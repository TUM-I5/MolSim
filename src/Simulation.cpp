#include "Simulation.h"

#include <spdlog/spdlog.h>

#include "LennardJones.h"
#include "utils/ArrayUtils.h"

namespace simulation {
namespace {

void CalculateX(ParticleContainer &particles, const double delta_t) {
    for (auto p : particles) {
        p.SetX(p.x() + delta_t * p.v() + (delta_t * delta_t / (2 * p.m())) * p.f());
    }
}

/**
 * @brief Computes LJ for each particle pair
 *
 * computed once per pair and then applies to both using newtons third law: F_ij = - F_ji
 *
 * @param particles particles to compute the forces for
 */
void CalculateF(ParticleContainer &particles) {
    // store old force in old_f and reset it, as it is calculated in the next step
    for (auto p : particles) {
        p.SetOldF(p.f());
        p.SetF(Vec3{});
    }

    particles.ApplyPairForces(LennardJones{});
}

void CalculateV(ParticleContainer &particles, const double delta_t) {
    for (auto p : particles) {
        p.SetV(p.v() + delta_t * (p.f() + p.old_f()) / (2 * p.m()));
    }
}
}  // namespace

void Simulation::Run(
    const std::function<void(const ParticleContainer &, int, double)> &post_iteration) {
    const int iteration_count = IterationCount();

    SPDLOG_DEBUG("Starting simulation: t = {} .. {}, dt = {}, {} iterations, {} particles",
                 settings_.start_time, settings_.end_time, settings_.delta_t, iteration_count,
                 particles_.size());

    CalculateF(particles_);

    post_iteration(particles_, 0, settings_.start_time);

    // for this loop, we assume: current x, current f and current v are known
    for (int iteration = 1; iteration <= iteration_count; ++iteration) {
        CalculateX(particles_, settings_.delta_t);
        CalculateF(particles_);
        CalculateV(particles_, settings_.delta_t);

        const double cur_time = settings_.start_time + settings_.delta_t * iteration;
        SPDLOG_TRACE("Iteration {} completed; t = {}", iteration, cur_time);

        post_iteration(particles_, iteration, cur_time);
    }
    SPDLOG_DEBUG("Simulation done");
}
int Simulation::IterationCount() const {
    return static_cast<int>(
        std::llround((settings_.end_time - settings_.start_time) / settings_.delta_t));
}
}  // namespace simulation
