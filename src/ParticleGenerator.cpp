//
// Created by Layla Zadina on 09.11.2023.
//

#include "ParticleGenerator.h"
#include "Particle.h"
#include "ParticleContainer.h"

ParticleGenerator::ParticleGenerator(int n1, int n2, int n3, double h1, double m1, std::array<double, 3> &v1, double x1, double y1, double z1, int type1)
        : v(v)
{
/** Number of particles per dimension: N1 × N2 × N3
// Distance h of the particles (mesh width of the grid)
// Mass m of one particle
// Initial velocity v of the particles (3 Components)
 The coordinate of the lower left front-side corner (x,y,z)
 */

// TODO: The mean-value of the velocity of the Brownian Motion - ?

N1 = n1;
N2 = n2;
N3 = n3;
h = h1;
m = m1;
v = v1;
x = x1;
y = y1;
z = z1;
type = type1;


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




