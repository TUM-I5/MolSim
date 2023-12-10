#include "Model.h"
#include "utils/ForceCalculations.h"

Model::Model(ParticleContainer &particleContainer, const std::string& forceType, const double delta_t)
        : delta_t(delta_t), particleContainer(particleContainer) {


    if(forceType == "LennJones") {
        //preliminary hardcoded
        double sigma{1.0};
        double epsilon{5.0};
        forceLambda = forceLennJonesPotentialFunction();

    } else if(forceType == "simple") {
        forceLambda = forceSimpleGravitational();

    } else {
        throw std::runtime_error("Provided forceType is invalid: " + forceType);
    }
}

void Model::calculateF()
{
    static std::pair<Particle *, Particle *> pair = std::make_pair(nullptr, nullptr);
    particleContainer.setNextPair(pair);

    while (pair.first != nullptr)
    {
        auto F_ij = forceLambda(*(pair.first), *(pair.second), {0,0,0});

        for (int k = 0; k < 3; k++)
        {
            pair.first->addF(k, F_ij[k]);
            pair.second->addF(k, -F_ij[k]);
        }

        particleContainer.setNextPair(pair);

    }

}

void Model::calculateX()
{
    Particle *p = particleContainer.getNextParticle();

    while (p != nullptr)
    {
        const std::array<double, 3> &old_x = p->getX();
        const std::array<double, 3> &v = p->getV();
        const std::array<double, 3> &f = p->getOldF();
        const double &m = p->getM();

        for (int i = 0; i < 3; i++)
        {
            p->setX(i, old_x[i] + delta_t * v[i] + delta_t * delta_t * f[i] / 2.0 / m);
        }

        p = particleContainer.getNextParticle();

    }
}

void Model::calculateV()
{
    Particle *p = particleContainer.getNextParticle();

    while (p != nullptr)
    {
        const std::array<double, 3> &v = p->getV();
        const std::array<double, 3> &f = p->getF();
        const std::array<double, 3> &f_old = p->getOldF();
        const double &m = p->getM();

        for (int i = 0; i < 3; i++)
        {
            p->setV(i, v[i] + delta_t * (f[i] + f_old[i]) / (2 * m));
        }

        p = particleContainer.getNextParticle();

    }
}

void Model::shiftForces()
{
    Particle *p = particleContainer.getNextParticle();
    while (p != nullptr)
    {
        p->shiftF();
        p = particleContainer.getNextParticle();
    }
}