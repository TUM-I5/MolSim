/*
 * DirectSumParticleContainer.cpp
 *
 * Created: 31.10.2022
 * Author:  wohlrapp
 */

#include "DirectSumParticleContainer.h"

DirectSumParticleContainer::DirectSumParticleContainer()
{
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("DirectSumParticleContainer generated!");
}

DirectSumParticleContainer::~DirectSumParticleContainer()
{
    _memoryLogger->info("DirectSumParticleContainer destructed!");
}

const void DirectSumParticleContainer::iterateParticles(std::function<void(Particle &)> f)
{
    for (Particle &p : particles)
    {
        f(p);
    }
};

const void DirectSumParticleContainer::iterateParticleInteractions(std::function<void(Particle &, Particle &)> f)
{
    // Since we use a vector we can directly access the particles through indexing
    // Because f_ij = -f_ji, we can save (n^2)/2 iterations by starting the inner loop at i+1
    for (long unsigned int i = 0; i < particles.size(); i++)
    {
        for (long unsigned int j = i + 1; j < particles.size(); j++)
        {
            f(particles[i], particles[j]);
        }
    }
}

const int DirectSumParticleContainer::size() const { return particles.size(); }

const void DirectSumParticleContainer::resetParticles()
{
    particles.clear();
}

const void DirectSumParticleContainer::addParticle(std::array<double, 3> &x, std::array<double, 3> &v, double &m, double &epsilon, double &sigma)
{
    // using the std function to create the new particle so it does not need to be copied to the right memory address
    particles.emplace_back(x, v, m, epsilon, sigma);
}

const void DirectSumParticleContainer::addParticle(std::array<double, 3> &x, std::array<double, 3> &v, double &m, double &epsilon, double &sigma, int &type)
{
    // using the std function to create the new particle so it does not need to be copied to the right memory address
    particles.emplace_back(x, v, m, epsilon, sigma, type);
}

const void DirectSumParticleContainer::addParticle(std::array<double, 3> &x, std::array<double, 3> &v, std::array<double, 3> &f, std::array<double, 3> &old_f, double &m, double &epsilon, double &sigma, int &type)
{

    particles.emplace_back(x, v, f, old_f, m, epsilon, sigma, type);
}

const void DirectSumParticleContainer::reserveMemoryForParticles(int numberOfParticles)
{
    // reserving for extra space before new particles are added. Push_back can then be executed without resizing
    int newLength = numberOfParticles + particles.size();
    particles.reserve(newLength);
}

std::vector<Particle> &DirectSumParticleContainer::getActiveParticles() { return particles; }
