//
// Created by Layla Zadina on 09.11.2023.
//

#include "ParticleGenerator.h"
#include "Particle.h"
#include "ParticleContainer.h"

ParticleGenerator::ParticleGenerator(int N1, int N2, int N3, double h, double m, std::array<double, 3> &v, double x, double y, double z, int type)
        : v(v)
{
/** Number of particles per dimension: N1 × N2 × N3
// Distance h of the particles (mesh width of the grid)
// Mass m of one particle
// Initial velocity v of the particles (3 Components)
 The coordinate of the lower left front-side corner (x,y,z)
 */

// TODO: The mean-value of the velocity of the Brownian Motion - ?

N1 = N1;
N2 = N2;
N3 = N3;
h = h;
m = m;
v = v;
x = x;
y = y;
z = z;
type = type;


auto particleContainer = new ParticleContainer();

    /**
        * create a N1xN2xN3 grid of particles with mass m, initial velocity v, left corner coordinate (x,y,z)
       */

    for (int i = 0; i < N1; ++i) {
        for (int j = 0; j < N2; ++j) {
            for (int k = 0; k < N3; ++k) {
                Particle particle({x + i*h, y + j*h, z + k*h}, v, m, type); // TODO: what is type?
                particleContainer->addParticle(particle);
            }
        }
    }

}




